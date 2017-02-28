#include "OGLSquare.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>


OGLSquare::OGLSquare()
{
}


OGLSquare::~OGLSquare()
{
}

void OGLSquare::draw(glm::vec3 pos, float r)
{
	// matrix
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float)640 / (float)480, 0.1f, 500.0f);

	// Or, for an ortho camera :
	//glm::mat4 Projection = glm::ortho(-320.0f,320.0f,-240.0f,240.0f,0.0f,100.0f); // In world coordinates
	//glm::mat4 Projection = glm::ortho(-12.0f, 12.0f, -9.0f, 9.0f, 0.0f, 100.0f); // In world coordinates

	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(0, 0, 3), // Camera is at (0,0,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	// Model matrix : an identity matrix (model will be at the origin)
	// todo: set position here as a translation, r as a scale
	glm::mat4 Model = glm::translate(pos) * glm::scale(glm::vec3(r, r, r)) * glm::mat4(1.0f);
	

	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 mvp = Projection * View * Model; // Remember, matrix multiplication is the other way around
											   // Draw our first triangle
	glUseProgram(mShaderProg);
	// Get a handle for our "MVP" uniform
	// Only during the initialization
	GLuint MatrixID = glGetUniformLocation(mShaderProg, "MVP");

	// Send our transformation to the currently bound shader, in the "MVP" uniform
	// This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

	glBindVertexArray(VAO);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// draw points 0-4 from the currently bound VAO with current in-use shader
	glDrawArrays(GL_QUADS, 0, 4);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	//glDrawArrays(GL_TRIANGLES, 1, 3);
	glBindVertexArray(0);
}

void OGLSquare::create()
{
	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] = {
		0.5f, -0.5f, 0.0f, // Right 
		0.5f, 0.5f, 0.0f, // RU
		-0.5f, 0.5f, 0.0f, // LU
		-0.5f, -0.5f, 0.0f, // Left  
		0.5f, -0.5f, 0.0f, // Right 
		0.5f, 0.5f, 0.0f, // RU
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)
}

void OGLSquare::LinkShaders()
{
	mShaderProg = OGLObj::LinkShaders(vertexShaderSource, fragmentShaderSource);
}
