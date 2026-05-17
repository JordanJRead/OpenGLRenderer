#ifndef CHANGEABLE_H
#define CHANGEABLE_H

template <typename T>
class Changeable {
public:
	Changeable(const T& initialValue) : mValue{ initialValue }
	{
	}

	Changeable() = default;

	bool hasChanged() {
		if (mValue != mPrevValue || mIsFirstCheck) {
			mPrevValue = mValue;
			mIsFirstCheck = false;
			return true;
		}
		return false;
	}

	T mValue;

private:
	T mPrevValue;
	bool mIsFirstCheck{ true };
};

#endif