#include <iostream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"

int main()
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	setGLVersionCORE(3, 3);
	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* Sets the frame rate */
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

		Renderer renderer;

		float r = 0.0f;
		float factor = 0.5f;
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();

			shader.Bind();
			shader.SetUniform4f("u_color", r, 0.3f, 0.8f, 1.0f);

			renderer.Draw(va, ib, shader);

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