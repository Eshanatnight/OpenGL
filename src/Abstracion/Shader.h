#pragma once
#include <string>
#include <unordered_map>

typedef unsigned int uint;

/* Shader File Seperator */
struct ShaderProgramSource
{
	std::string VertexShader;
	std::string FragmentShader;
};

/* Shader Class */
class Shader
{
private:
	std::string m_FilePath;    // For Debuging
	uint m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache; // caching for uniform

public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void SetUniform4f(const std::string name, float v0, float v1, float v2, float v3);  	// Set Uniform

private:
	uint createShader(const std::string& vertexShader, const std::string& fragmentShader);
	int GetUniformLocation(const std::string& name);
	uint compileShader(uint type, const std::string& source);
	ShaderProgramSource ParseShader(const std::string& filePath);
};
