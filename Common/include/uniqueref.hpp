#ifndef UNIQUE_REF_HPP
#define UNIQUE_REF_HPP

#include <memory>
#include <stdexcept>

template <typename T>
class UniqueRef {
public:
    UniqueRef(std::unique_ptr<T>&& ptr) : mData{ std::move(ptr) } {
        if (mData == nullptr) {
            throw std::runtime_error(
              "Cannot create a UniqueRef from a nullptr!");
        }
    }

    T* ptr() {
        return mData.get();
    }

    const T* ptr() const {
        return mData.get();
    }

    T& operator*() {
        return *mData;
    }

    const T& operator*() const noexcept {
        return *mData;
    }

    T* operator->() noexcept {
        return mData.get();
    }

    const T* operator->() const noexcept {
        return mData.get();
    }

    operator T&() noexcept {
        return *mData;
    }

    operator T&() const noexcept {
        return *mData;
    }

private:
    std::unique_ptr<T> mData;
};

#endif