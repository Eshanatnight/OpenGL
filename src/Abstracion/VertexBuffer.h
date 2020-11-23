#pragma once
typedef unsigned int uint;

/* Vertex Buffer Class */
class VertexBuffer
{
private:
	uint m_RendererID;  //? ID for a specific object

public:
	VertexBuffer(const void* data, uint size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};
