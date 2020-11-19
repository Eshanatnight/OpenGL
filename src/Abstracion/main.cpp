#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "main.h"

struct ShaderProgramSource
{
	std::string VertexShader;
	std::string FragmentShader;
};

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

static ShaderProgramSource ParseShader(const std::string& filePath)
{
	std::ifstream stream(filePath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;

			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}

		else
		{
			ss[(int)type] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str() };
}

static unsigned int compileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	GlCall(glShaderSource(id, 1, &src, nullptr));
	GlCall(glCompileShader(id));

	// Error Handling
	int result;
	GlCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		int length;
		GlCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));

		char* message = (char*)alloca(length * sizeof(char));
		GlCall(glGetShaderInfoLog(id, length, &length, message));
		std::cout << "Failed To Compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;
		GlCall(glDeleteShader(id));
		return 0;
	}
	return id;
}

static unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	GlCall(glAttachShader(program, vs));
	GlCall(glAttachShader(program, fs));
	GlCall(glLinkProgram(program));
	GlCall(glValidateProgram(program));

	GlCall(glDeleteShader(vs));
	GlCall(glDeleteShader(fs));

	return program;
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
		unsigned int indices[] =
		{
			0, 1, 2,
			2, 3, 0
		};

		/* Vertex Arrays Objects */
		unsigned int vao;
		GlCall(glGenVertexArrays(1, &vao));
		GlCall(glBindVertexArray(vao));

		/* Array Buffer Object */
		VertexBuffer vb(positions, 4 * 2 * sizeof(float));

		GlCall(glEnableVertexAttribArray(0));
		GlCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

		/* Index Buffer Object */
		IndexBuffer ib(indices, 6);

		ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");

		unsigned int shader = createShader(source.VertexShader, source.FragmentShader);
		GlCall(glUseProgram(shader));

		GlCall(int location = glGetUniformLocation(shader, "u_color"));
		ASSERT(location != -1);
		GlCall(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f));

		/* clears the state */
		GlCall(glBindVertexArray(0));
		GlCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
		GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		GlCall(glUseProgram(0));

		float r = 0.0f;
		float factor = 0.5f;
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			GlCall(glClear(GL_COLOR_BUFFER_BIT));

			GlCall(glBindVertexArray(vao));
			ib.Bind();

			GlCall(glUseProgram(shader));
			GlCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));
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
		GlCall(glDeleteProgram(shader));
	}

	glfwTerminate();
	return 0;
}