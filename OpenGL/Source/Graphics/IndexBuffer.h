#pragma once
#include <GL/glew.h>

class IndexBuffer
{
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	unsigned int GetCount() const;

private:
	unsigned int mBufferID;
	unsigned int mCount;
};

