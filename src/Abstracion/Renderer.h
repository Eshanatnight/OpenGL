#pragma once
#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

#define ASSERT(x) if (!(x)) __debugbreak();

#define GlCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

typedef unsigned int uint;    // ! for ease of use

/* Clears GL Error List */
void GLClearError();

/* For Setting up the OpenGl Version and Profile */
void setGLVersionCORE(const int& major, const int& minor);
void setGLVersionCOMPAT(const int& major, const int& minor);

/* Logs Errors to Console */
bool GLLogCall(const char* function, const char* file, int line);

/* Sets Render Framerate */
void setFrameRate(const uint& frmRate);

/* Renderer Class */
class Renderer
{
public:
	void Clear() const;
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};
