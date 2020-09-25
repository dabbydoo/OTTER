#include "Shader.h"
#include "Logging.h"
#include <fstream>
#include <sstream>

Shader::Shader() :
	_vs(0),
	_fs(0),
	_handle(0)
{
	// TODO: Constructor
	_handle = glCreateProgram();
}

Shader::~Shader() {
	// TODO: Destructor
	if (_handle != 0) 
	{ 
		glDeleteProgram(_handle);    
		_handle = 0; 
	}
}

bool Shader::LoadShaderPart(const char* source, GLenum type)
{
	// TODO: Implement

	// Creates a new shader part (VS, FS, GS, etc...)    
	GLuint handle = glCreateShader(type);

	// Load the GLSL source and compile it    
	glShaderSource(handle, 1, &source, nullptr);    
	glCompileShader(handle);

	// Get the compilation status for the shader part    
	GLint status = 0;    
	glGetShaderiv(handle, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE) {
		// Get the size of the error log
		GLint logSize = 0;
		glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &logSize);

		// Create a new character buffer for the log
		char* log = new char[logSize];

		// Get the log
		glGetShaderInfoLog(handle, logSize, &logSize, log);

		// Dump error log
		LOG_ERROR("Failed to compile shader part:\n{}", log);

		// Clean up our log memory
		delete[] log;

		// Delete the broken shader result
		glDeleteShader(handle);
		handle = 0;
	}
}

bool Shader::LoadShaderPartFromFile(const char* path, GLenum type) {
	std::ifstream file(path);
	std::stringstream stream;
	stream << file.rdbuf();
	bool result = LoadShaderPart(stream.str().c_str(), type);
	file.close();
	return result;
}

bool Shader::Link()
{
	// Todo: Implement
}

void Shader::Bind() {
	glUseProgram(_handle);
}

void Shader::UnBind() {
	glUseProgram(0);
}
