#ifndef VIEWING_H
#define VIEWING_H

#include <vector>

template <typename T>
class Viewable;

// VIEWER DECLARATION
template <typename T>
class Viewer {
	friend Viewable<T>;
public:
	T* get();
	void lookAt(Viewable<T>* viewable);
	~Viewer();

private:
	Viewable<T>* mViewable{};
};

// VIEWABLE DECLARATION
template <typename T>
class Viewable {
	friend Viewer<T>;
public:
	Viewable(T* owner);
	~Viewable();

private:
	std::vector<Viewer<T>*> mViewers;
	T* mOwner;

	void removeViewer(Viewer<T>* viewer);
	void registerViewer(Viewer<T>* viewer);
};

// VIEWER DEFINITIONS
template <typename T>
T* Viewer<T>::get() {
	if (mViewable) {
		return mViewable->mOwner;
	}
	return nullptr;
}

template <typename T>
void Viewer<T>::lookAt(Viewable<T>* viewable) {
	if (mViewable) {
		mViewable->removeViewer(this);
	}
	if (!viewable) {
		mViewable = nullptr;
	}
	else {
		mViewable = viewable;
		mViewable->registerViewer(this);
	}
}

template <typename T>
Viewer<T>::~Viewer() {
	if (mViewable) {
		mViewable->removeViewer(this);
	}
}

// VIEWABLE DEFINITIONS
template <typename T>
Viewable<T>::Viewable(T* owner) : mOwner{ owner } {}

template <typename T>
Viewable<T>::~Viewable() {
	for (Viewer<T>* viewer : mViewers) {
		viewer->mViewable = nullptr;
	}
}

template <typename T>
void Viewable<T>::removeViewer(Viewer<T>* viewer) {
	std::erase(mViewers, viewer);
}

template <typename T>
void Viewable<T>::registerViewer(Viewer<T>* viewer) {
	mViewers.push_back(viewer);
}

#endif