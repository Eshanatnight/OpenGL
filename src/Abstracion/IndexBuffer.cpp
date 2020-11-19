#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const uint* data, uint count)
	:m_count(count)
{
	ASSERT(sizeof(uint) == sizeof(GLuint));

	GlCall(glGenBuffers(1, &m_RendererID));
	GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	GlCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	GlCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind() const
{
	GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::Unbind() const
{
	GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}