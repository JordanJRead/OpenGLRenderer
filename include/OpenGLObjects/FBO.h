#ifndef FBO_H
#define FBO_H

#include <glad/glad.h>

/// <summary>
/// Wrapper class for automatically managing OpenGL object memory
/// </summary>
class FBO {
public:
	FBO() { glGenFramebuffers(1, &mID); }
	~FBO() { glDeleteFramebuffers(1, &mID); }
	operator unsigned int() const { return mID; }
	void use(int target) const { glBindFramebuffer(target, mID); }

	FBO(const FBO&) = delete;
	FBO(FBO&&) = delete;
	FBO& operator=(const FBO&) = delete;
	FBO& operator=(FBO&&) = delete;

private:
	unsigned int mID;
};

#endif