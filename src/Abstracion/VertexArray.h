#pragma once
#include "VertexBuffer.h"

class VertexBufferLayout;

typedef unsigned int uint;

/* Class Vertex Array */
class VertexArray
{
private:
	uint m_RendererID;   //? ID for a specific object

public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
	void Bind() const;
	void Unbind() const;
};
