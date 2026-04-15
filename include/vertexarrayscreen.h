#ifndef VERTEX_ARRAY_SCREEN_H
#define VERTEX_ARRAY_SCREEN_H

#include "vertexarray.h"

class VertexArrayScreen {
public:
	VertexArrayScreen() {
		mVertexArray.create({
			-1, -1, 0, 0,
			1, -1, 1, 0,
			-1, 1, 0, 1,
			1, 1, 1 ,1
			}, { 0, 1, 2, 1, 3, 2 }, { 2, 2 });
	}

	void bind() const {
		mVertexArray.bind();
	}
	size_t getIndexCount() const {
		return mVertexArray.getIndexCount();
	}

private:
	VertexArray mVertexArray;
};

#endif