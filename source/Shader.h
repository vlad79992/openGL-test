#pragma once

#include <string>
#include <unordered_map>

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
public:
	
	Shader(const std::string& filename);
	~Shader();

	void Bind() const;
	void Unbind() const;

	//set uniforms
	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform4f(const std::string& name, \
		float f0, float f1, float f2, float f3);

private:
	unsigned int m_RendererID;
	std::string m_filepath;
	std::unordered_map<std::string, int> m_UniformLocationCache;

	ShaderProgramSource ParseShader(std::string filepath);

	unsigned int CompileShader(unsigned int type, const std::string& source);
	
	unsigned int CreateShader(const std::string& shaderFilePath);
	
	int GetUniformLocation(const std::string& name);

};