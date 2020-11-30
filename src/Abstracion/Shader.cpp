#include "Shader.h"
#include "Renderer.h"

#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string& filepath)
	:m_FilePath(filepath), m_RendererID(0)
{
	ShaderProgramSource source = ParseShader(filepath);
	m_RendererID = createShader(source.VertexShader, source.FragmentShader);
}

Shader::~Shader()
{
	GlCall(glDeleteProgram(m_RendererID));
}

uint Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	uint program = glCreateProgram();
	uint vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	uint fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	GlCall(glAttachShader(program, vs));
	GlCall(glAttachShader(program, fs));
	GlCall(glLinkProgram(program));
	GlCall(glValidateProgram(program));

	GlCall(glDeleteShader(vs));
	GlCall(glDeleteShader(fs));

	return program;
}

uint Shader::compileShader(uint type, const std::string& source)
{
	uint id = glCreateShader(type);
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

		/* alloca "can" cause Stack Overflow Exeption
			Possible Alternatives
			1. using _malloca (For Stack Allocation)
			2. Heap allocating with new (might cause slow down if used repeatedly for modification of the same data buffer
		*/
		char* message = (char*)alloca(length * sizeof(char));
		GlCall(glGetShaderInfoLog(id, length, &length, message));
		std::cout << "Failed To Compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;
		GlCall(glDeleteShader(id));
		return 0;
	}
	return id;
}

ShaderProgramSource Shader::ParseShader(const std::string& filePath)
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

void Shader::Bind() const
{
	GlCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
	GlCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string name, int value)
{
	GlCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string name, float value)
{
	GlCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string name, float v0, float v1, float v2, float v3)
{
	GlCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];

	GlCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	if (location == -1)
		std::cout << "Warning: Uniform " << name << " does not exist!" << std::endl;
	m_UniformLocationCache[name] = location;
	return location;
}