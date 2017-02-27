#ifndef _SPHERE_H
#define _SPHERE_H

#include <GL/glew.h>

class Sphere
{
public:
	Sphere();
	~Sphere();
	void init(GLuint vertexPositionID);
	void cleanup();
	void draw();
	GLuint loadShaders(const char *vertFile, const char *fragFile);

public:
	int lats, longs;
	bool isInited;
	GLuint m_vao, m_vboVertex, m_vboIndex, mProgramId, mMVPId;;
	int numsToDraw;
};

#endif