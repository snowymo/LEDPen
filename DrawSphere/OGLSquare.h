#ifndef _OGL_SQUARE_H
#define _OGL_SQUARE_H

#include <glm/glm.hpp>

#include "OGLObj.h"
class OGLSquare :
	public OGLObj
{
public:
	OGLSquare();
	~OGLSquare();

	void draw(glm::vec3 pos = glm::vec3(0,0,0), float r = 1);
	void create();
	void LinkShaders();

private:
	const GLchar* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 position;\n"
		"uniform mat4 MVP;\n"
		"void main()\n"
		"{\n"
		"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
		"gl_Position =  MVP * vec4(position,1);\n"
		"}\0";
	const GLchar* fragmentShaderSource = "#version 330 core\n"
		"out vec4 color;\n"
		"void main()\n"
		"{\n"
		"color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\n\0";
};

#endif