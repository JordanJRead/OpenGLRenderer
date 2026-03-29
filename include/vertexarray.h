#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include <vector>
#include <glad/glad.h>
#include <OpenGLObjects/BUF.h>
#include <OpenGLObjects/VAO.h>

class VertexArray {
public:
	/// <summary>
	/// Creates a vertex array object
	/// </summary>
	/// <param name="vertexData">The raw vertex data</param>
	/// <param name="vertexIndices">The vertex indices</param>
	/// <param name="vertexLayout">A vector that specifies how many float components are in each attribute</param>
	void create(const std::vector<float>& vertexData, const std::vector<unsigned int>& vertexIndices, const std::vector<int>& vertexLayout);

	/// <summary>
	/// Binds this vertex array object
	/// </summary>
	void bind() const { glBindVertexArray(mVAO); }

	/// <summary>
	/// Gets the number of indices
	/// </summary>
	size_t getIndexCount() const { return mIndexCount; }

private:
	VAO mVAO;
	BUF mVBO;
	BUF mEBO;
	size_t mIndexCount{};
};

#endif