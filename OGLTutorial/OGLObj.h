#ifndef _OGLOBJ_H
#define _OGLOBJ_H

#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library


class OGLObj
{
public:
	OGLObj();
	~OGLObj();
	virtual void draw(GLuint program_id);
	GLuint LinkShaders(const GLchar* vert, const GLchar* frag);
	GLuint buildShader(const GLchar* shader, GLenum type);
	virtual void create();

protected:
	GLuint VBO, VAO;
	GLuint mShaderProg;


};

#endif