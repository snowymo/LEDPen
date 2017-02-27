#include "OGLSphere.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>


OGLSphere::OGLSphere()
{
	isInited = false;
	m_vboIndex = 0;
	
	lats = 4;
	longs = 4;
}


OGLSphere::~OGLSphere()
{
}

void OGLSphere::draw()
{
	// matrix
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);

	// Or, for an ortho camera :
	//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 mvp = Projection * View * Model; // Remember, matrix multiplication is the other way around
											   // Draw our first triangle
	glUseProgram(mShaderProg);
	// Get a handle for our "MVP" uniform
	// Only during the initialization
	GLuint MatrixID = glGetUniformLocation(mShaderProg, "MVP");

	if (!isInited) {
		std::cout << "please call init() before draw()" << std::endl;
	}

	// draw sphere
	glBindVertexArray(VAO);
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(GL_PRIMITIVE_RESTART_FIXED_INDEX);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIndex);
	glDrawElements(GL_QUAD_STRIP, numsToDraw, GL_UNSIGNED_INT, NULL);

}

void OGLSphere::create()
{
	int i, j;
	
	int indicator = 0;
	for (i = 0; i <= lats; i++) {
		double lat0 = glm::pi<double>() * (-0.5 + (double)(i - 1) / lats);
		double z0 = sin(lat0);
		double zr0 = cos(lat0);

		double lat1 = glm::pi<double>() * (-0.5 + (double)i / lats);
		double z1 = sin(lat1);
		double zr1 = cos(lat1);

		for (j = 0; j <= longs; j++) {
			double lng = 2 * glm::pi<double>() * (double)(j - 1) / longs;
			double x = cos(lng);
			double y = sin(lng);

			vertices.push_back(x * zr0);
			vertices.push_back(y * zr0);
			vertices.push_back(z0);
			indices.push_back(indicator);
			indicator++;

			vertices.push_back(x * zr1);
			vertices.push_back(y * zr1);
			vertices.push_back(z1);
			indices.push_back(indicator);
			indicator++;
		}
		indices.push_back(GL_PRIMITIVE_RESTART_FIXED_INDEX);
	}

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

	// pass parameter
//  	GLuint positionAttrId = glGetAttribLocation(mShaderProg, "position");
//  	glVertexAttribPointer(positionAttrId, 3, GL_FLOAT, GL_FALSE, 0, NULL);
//  	glEnableVertexAttribArray(positionAttrId);
// 
 	glGenBuffers(1, &m_vboIndex);
 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIndex);
 	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	numsToDraw = indices.size();

	isInited = true;
}

void OGLSphere::LinkShaders()
{
	mShaderProg = OGLObj::LinkShaders(vertexShaderSource, fragmentShaderSource);
}
