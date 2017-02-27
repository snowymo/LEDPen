#include "OGLObj.h"
#include <iostream>


OGLObj::OGLObj()
{
}


OGLObj::~OGLObj()
{
}

void OGLObj::draw(GLuint program_id)
{

}

GLuint OGLObj::LinkShaders(const GLchar* vert, const GLchar* frag)
{
	// Check for compile time errors
	GLint success;
	GLchar infoLog[512];
	GLuint vertShader = buildShader(vert, GL_VERTEX_SHADER);
	GLuint fragShader = buildShader(frag, GL_FRAGMENT_SHADER);
	mShaderProg = glCreateProgram();
	glAttachShader(mShaderProg, vertShader);
	glAttachShader(mShaderProg, fragShader);
	glLinkProgram(mShaderProg);
	// Check for linking errors
	glGetProgramiv(mShaderProg, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(mShaderProg, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertShader);
	glDeleteShader(fragShader);
	return mShaderProg;
}

GLuint OGLObj::buildShader(const GLchar* shader, GLenum type)
{
	// Build and compile our shader program
	// Vertex shader
	GLuint shaderId = glCreateShader(type);
	glShaderSource(shaderId, 1, &shader, NULL);
	glCompileShader(shaderId);
	// Check for compile time errors
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	return shaderId;
}
