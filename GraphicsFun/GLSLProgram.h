#pragma once

#include <string>
#include <GL/glew.h>

class GLSLProgram
{
public:
	GLSLProgram();
	~GLSLProgram();

	void compileShaders(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	void linkShaders();
	void addAttribute(const std::string& attribName);

	void use();
	void unuse();

	GLint getUniformLocation(const std::string& uniformName);

private:
	void compileShader(const std::string filePath, GLuint& id);

	int _numAttributes;

	// Shader IDs
	GLuint _programID;
	GLuint _vertShaderID;
	GLuint _fragShaderID;
};

