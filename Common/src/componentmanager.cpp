#include "componentmanager.hpp"
#include "model.hpp"
#include "pointlight.hpp"

template <typename T>
std::unique_ptr<Component> createFromJSON(const JSON& json) {
	return std::make_unique<T>(json);
}

ComponentManager& ComponentManager::instance() {
	static ComponentManager manager;
	return manager;
}

ComponentManager::ComponentManager() {
	mStaticComponentFactories["Model"] = createFromJSON<Model>;
	mStaticComponentFactories["PointLight"] = createFromJSON<PointLight>;
}

std::unique_ptr<Component> ComponentManager::createComponentFromName(std::string_view componentTypeNameView, const JSON* const json) {
	std::string componentTypeName{ componentTypeNameView };
	// Static
	if (json && mStaticComponentFactories.contains(componentTypeName)) {
		auto& func = mStaticComponentFactories.at(componentTypeName);
		return func(*json);
	}

	// Dynamic
	if (json && mDynamicComponentFactories.contains(componentTypeName)) {
		return mDynamicComponentFactories.at(componentTypeName)(*json);
	}
	return nullptr;
}

// TODO move to new file?
ComponentManager::Iterator ComponentManager::begin() {
	return Iterator{ this, mStaticComponentFactories.begin(), true };
}

ComponentManager::Iterator ComponentManager::end() {
	return Iterator{ this, mDynamicComponentFactories.end(), false };
}

ComponentManager::Iterator::reference ComponentManager::Iterator::operator*() const {
    return mMapIterator->first;
}

ComponentManager::Iterator& ComponentManager::Iterator::operator++() {
	mMapIterator++;
    if (mInStaticNames && mMapIterator == mOwner->mStaticComponentFactories.end()) {
        // Go into dynamic
        mMapIterator = mOwner->mDynamicComponentFactories.begin();
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

ComponentManager::Iterator::Iterator(ComponentManager* owner, std::map<std::string, std::function<std::unique_ptr<Component>(const JSON&)>>::iterator iterator, bool inStatic)
    : mOwner{ owner }
    , mMapIterator{ iterator }
    , mInStaticNames{ inStatic }
{
}