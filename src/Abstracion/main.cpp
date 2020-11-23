#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

/*
	// Important
	Sets the frame rate via LValue references
	uses a const uint reference instead of an LValue Reference
	so in the event the frame rate needs to be set by a RValue
	without the use of the move assignment operator
*/
void setFrameRate(const uint& frmRate)
{
	glfwSwapInterval(frmRate);
}

int main()
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* For Setting up the OpenGl Version and Profile */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	uint rate = 5;
	setFrameRate(rate);

	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;
	{
		/* Vertex Buffer */
		float positions[] =
		{
			-0.5f, -0.5f,
			0.5f, -0.5f,
			0.5f, 0.5f,
			-0.5f, 0.5f
		};

		/* Index Buffer */
		uint indices[] =
		{
			0, 1, 2,
			2, 3, 0
		};

		/* Vertex Arrays Objects */
		VertexArray va;

		/* Array Buffer Object */
		VertexBuffer vb(positions, 4 * 2 * sizeof(float));

		/* Vertex Buffer Layout */
		VertexBufferLayout layout;
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);

		/* Index Buffer Object */
		IndexBuffer ib(indices, 6);

		Shader shader("res/shaders/Basic.shader");
		shader.Bind();
		shader.SetUniform4f("u_color", 0.8f, 0.3f, 0.8f, 1.0f);

		/* clears the state i.e. unbind */
		va.Unbind();
		vb.Unbind();
		ib.Unbind();
		shader.Unbind();

		float r = 0.0f;
		float factor = 0.5f;
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			GlCall(glClear(GL_COLOR_BUFFER_BIT));

			shader.Bind();
			shader.SetUniform4f("u_color", r, 0.3f, 0.8f, 1.0f);

			va.Bind();
			ib.Bind();

			GlCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

			if (r > 1.0f)
				factor = -0.5f;
			else if (r < 0.0f)
				factor = 0.5f;

			r += factor;

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}

	glfwTerminate();
	return 0;
}