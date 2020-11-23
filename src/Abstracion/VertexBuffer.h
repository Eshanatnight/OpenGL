#pragma once
typedef unsigned int uint;

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
