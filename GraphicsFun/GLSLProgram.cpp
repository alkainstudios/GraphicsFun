#include "GLSLProgram.h"
#include "Errors.h"
#include <fstream>
#include <vector>


GLSLProgram::GLSLProgram() : 
	_numAttributes(0), 
	_programID(0), 
	_vertShaderID(0), 
	_fragShaderID(0)
{
}


GLSLProgram::~GLSLProgram()
{
}

// ------------------------------------------------------------------------------------------
void GLSLProgram::compileShaders(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) {
	
	_vertShaderID = glCreateShader(GL_VERTEX_SHADER);
	if (_vertShaderID == 0) {
		fatalError("Vertex shader failed to be created!");
	}

	_fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	if (_fragShaderID == 0) {
		fatalError("Fragment shader failed to be created!");
	}

	compileShader(vertexShaderPath, _vertShaderID);
	compileShader(fragmentShaderPath, _fragShaderID);
}

// ------------------------------------------------------------------------------------------
void GLSLProgram::linkShaders() {
	//Vertex and fragment shaders are successfully compiled.
	//Now time to link them together into a program.
	//Get a program object.
	_programID = glCreateProgram();

	//Attach our shaders to our program
	glAttachShader(_programID, _vertShaderID);
	glAttachShader(_programID, _fragShaderID);

	//Link our program
	glLinkProgram(_programID);

	//Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(_programID, GL_LINK_STATUS, (int *)&isLinked);

	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::vector<char> errorLog(maxLength);
		glGetProgramInfoLog(_programID, maxLength, &maxLength, &errorLog[0]);

		//We don't need the program anymore.
		glDeleteProgram(_programID);
		//Don't leak shaders either.
		glDeleteShader(_vertShaderID);
		glDeleteShader(_fragShaderID);

		//Prints the errorLog to the console.

		std::printf("%s\n", &(errorLog[0]));
		fatalError("Shader failed to link!");
	}

	//Always detach shaders after a successful link.
	glDetachShader(_programID, _vertShaderID);
	glDetachShader(_programID, _fragShaderID);
	glDeleteShader(_vertShaderID);
	glDeleteShader(_fragShaderID);
}

// ------------------------------------------------------------------------------------------
void GLSLProgram::addAttribute(const std::string& attribName) {
	glBindAttribLocation(_programID, _numAttributes++, attribName.c_str());
}

// ------------------------------------------------------------------------------------------
GLint GLSLProgram::getUniformLocation(const std::string& uniformName) {
	GLint location = glGetUniformLocation(_programID, uniformName.c_str());
	if (location == GL_INVALID_INDEX) {
		fatalError("Uniform " + uniformName + " not found in shader!");
	}
	return location;
}

// ------------------------------------------------------------------------------------------
void GLSLProgram::compileShader(const std::string filePath, GLuint& id) {


	std::ifstream shaderFile(filePath);
	if (shaderFile.fail()) {
		perror(filePath.c_str());
		fatalError("Failed to open " + filePath);
	}

	std::string fileContnets;
	std::string line;

	while (std::getline(shaderFile, line)) {
		fileContnets += line + "\n";
	}

	shaderFile.close();

	const char* contentsPtr = fileContnets.c_str();
	glShaderSource(id, 1, &contentsPtr, nullptr);

	glCompileShader(id);


	GLint success = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

		// Provide the infolog in whatever manor you deem best.
		// Exit with failure.
		glDeleteShader(id); // Don't leak the shader.

		std::printf("%s\n", &(errorLog[0]));
		fatalError("Shader "+ filePath +" failed to compile!");
	}
}


void GLSLProgram::use() {
	glUseProgram(_programID);
	for (int i = 0; i < _numAttributes; i++)
	{
		glEnableVertexAttribArray(i);
	}
}


void GLSLProgram::unuse() {
	glUseProgram(0);
	for (int i = 0; i < _numAttributes; i++)
	{
		glDisableVertexAttribArray(i);
	}
}