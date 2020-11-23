#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

typedef unsigned int uint;

class VertexArray
{
private:
	uint m_RendererID;

public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
	void Bind() const;
	void Unbind() const;
};
