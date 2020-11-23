#pragma once

#include <vector>
#include "Renderer.h"
#include "GL\glew.h"

struct VertexBufferElement
{
	uint type;
	uint count;
	unsigned char normalized;

	static uint GetSizeOfType(uint type)
	{
		switch (type)
		{
		case GL_FLOAT:			 return 4;
		case GL_UNSIGNED_INT:	 return 4;
		case GL_UNSIGNED_BYTE:	 return 1;
		}

		ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout
{

private:
	std::vector<VertexBufferElement> m_Element;
	uint m_Stride;

public:
	VertexBufferLayout()
		:m_Stride(0)
	{}

	template<typename T>
	void Push(uint count)
	{
		static_assert(false);
	}

	template<>
	void Push<float>(uint count)
	{
		m_Element.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;
	}

	template<>
	void Push<uint>(uint count)
	{
		m_Element.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
	}

	template<>
	void Push<unsigned char>(uint count)
	{
		m_Element.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
	}

	inline uint GetStride() const { return m_Stride; }

	inline const std::vector<VertexBufferElement>& GetElements() const { return m_Element; }



};
