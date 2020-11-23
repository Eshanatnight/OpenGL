#pragma once
#include <string>
#include <unordered_map>

typedef unsigned int uint;

struct ShaderProgramSource
{
	std::string VertexShader;
	std::string FragmentShader;
};

class Shader
{
private:
	// For Debuging
	std::string m_FilePath;
	uint m_RendererID;

	// caching for uniform
	std::unordered_map<std::string, int> m_UniformLocationCache;

public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	// Set Uniform
	void SetUniform4f(const std::string name, float v0, float v1, float v2, float v3);

private:
	uint createShader(const std::string& vertexShader, const std::string& fragmentShader);
	int GetUniformLocation(const std::string& name);
	uint compileShader(uint type, const std::string& source);
	ShaderProgramSource ParseShader(const std::string& filePath);
};
