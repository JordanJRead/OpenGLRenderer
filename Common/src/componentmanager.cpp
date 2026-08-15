#include "componentmanager.hpp"
#include "model.hpp"
#include "pointlight.hpp"

template <typename T>
std::unique_ptr<Component> createFromEmpty() {
	return std::make_unique<T>();
}

template <typename T>
std::unique_ptr<Component> createFromJSON(const JSON& json) {
	return std::make_unique<T>(json);
}

ComponentManager& ComponentManager::instance() {
	static ComponentManager manager;
	return manager;
}

ComponentManager::ComponentManager() {
	mStaticComponentEmptyFactories["Model"] = createFromEmpty<Model>;
	mStaticComponentEmptyFactories["Point Light"] = createFromEmpty<PointLight>;
	
	mStaticComponentJSONFactories["Model"] = createFromJSON<Model>;
	mStaticComponentJSONFactories["Point Light"] = createFromJSON<PointLight>;
}

std::unique_ptr<Component> ComponentManager::createComponentFromName(std::string_view componentTypeNameView, const JSON* const json) {
	std::string componentTypeName{ componentTypeNameView };
	// Static
	if (!json && mStaticComponentEmptyFactories.contains(componentTypeName)) {
		return mStaticComponentEmptyFactories.at(componentTypeName)();
	}
	if (json && mStaticComponentJSONFactories.contains(componentTypeName)) {
		return mStaticComponentJSONFactories.at(componentTypeName)(*json);
	}

	// Dynamic
	if (!json && mDynamicComponentEmptyFactories.contains(componentTypeName)) {
		return mDynamicComponentEmptyFactories.at(componentTypeName)();
	}
	if (json && mDynamicComponentJSONFactories.contains(componentTypeName)) {
		return mDynamicComponentJSONFactories.at(componentTypeName)(*json);
	}
	return nullptr;
}

// TODO move to new file?
ComponentManager::Iterator ComponentManager::begin() {
	return Iterator{ this, mStaticComponentEmptyFactories.begin(), true };
}

ComponentManager::Iterator ComponentManager::end() {
	return Iterator{ this, mDynamicComponentEmptyFactories.end(), false };
}

ComponentManager::Iterator::reference ComponentManager::Iterator::operator*() const {
    return mMapIterator->first;
}

ComponentManager::Iterator& ComponentManager::Iterator::operator++() {
	mMapIterator++;
    if (mInStaticNames && mMapIterator == mOwner->mStaticComponentEmptyFactories.end()) {
        // Go into dynamic
        mMapIterator = mOwner->mDynamicComponentEmptyFactories.begin();
		mInStaticNames = false;
    }
    return *this;
}

ComponentManager::Iterator ComponentManager::Iterator::operator++(int) {
    Iterator old = *this;
    ++(*this);
    return old;
}

bool ComponentManager::Iterator::operator==(const Iterator& other) const {
    return mInStaticNames == other.mInStaticNames && mMapIterator == other.mMapIterator;
}

bool ComponentManager::Iterator::operator!=(const Iterator& other) const {
    return !(*this == other);
}

ComponentManager::Iterator::Iterator(ComponentManager* owner, std::map<std::string, std::function<std::unique_ptr<Component>()>>::iterator iterator, bool inStatic)
    : mOwner{ owner }
    , mMapIterator{ iterator }
    , mInStaticNames{ inStatic }
{
}