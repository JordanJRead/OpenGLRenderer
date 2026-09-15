#ifndef UNIQUE_REF_HPP
#define UNIQUE_REF_HPP

#include <memory>
#include <stdexcept>

template <typename T>
class UniqueRef {
public:
    UniqueRef(std::unique_ptr<T>&& ptr) : mData{ std::move(ptr) } {
        checkInvariant();
    }

    void reassign(std::unique_ptr<T>&& ptr) {
        mData = std::move(ptr);
        checkInvariant();
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
    void checkInvariant() const {
        if (!mData) {
            throw std::runtime_error(
              "Cannot create a UniqueRef from a nullptr!");
        }
    }

private:
    std::unique_ptr<T> mData;
};

#endif