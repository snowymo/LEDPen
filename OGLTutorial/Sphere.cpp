#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Sphere.h"
#include <vector>
#include <iostream>
#include <fstream>


Sphere::Sphere()
{
	isInited = false;
	m_vao = 0;
	m_vboVertex = 0;
	m_vboIndex = 0;
	mProgramId = 0;
	mMVPId = 0;

	lats = 40;
	longs = 40;
}


Sphere::~Sphere()
{
}

void Sphere::init(GLuint vertexPositionID)
{
	int i, j;
	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;
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

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vboVertex);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboVertex);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(vertexPositionID, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(vertexPositionID);

	glGenBuffers(1, &m_vboIndex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIndex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	numsToDraw = indices.size();

	isInited = true;

	
}

void Sphere::cleanup()
{
	if (!isInited) {
		return;
	}
	if (m_vboVertex) {
		glDeleteBuffers(1, &m_vboVertex);
	}
	if (m_vboIndex) {
		glDeleteBuffers(1, &m_vboIndex);
	}
	if (m_vao) {
		glDeleteVertexArrays(1, &m_vao);
	}

	isInited = false;
	m_vao = 0;
	m_vboVertex = 0;
	m_vboIndex = 0;
}

void Sphere::draw()
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

											   // Only during the initialization
	mMVPId = glGetUniformLocation(mProgramId, "MVP");
	glUseProgram(mProgramId);
	// Send our transformation to the currently bound shader, in the "MVP" uniform
	// This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
	glUniformMatrix4fv(mMVPId, 1, GL_FALSE, &mvp[0][0]);
// 	GLuint m_projMatrix = glGetUniformLocation(mProgramId, "projMatrix");
// 	GLuint mvMatrixID = glGetUniformLocation(mProgramId, "mvMatrix");
// 	glUniformMatrix4fv(projMatrixID, 1, GL_FALSE, glm::value_ptr(m_projMatrix));
// 	glUniformMatrix4fv(mvMatrixID, 1, GL_FALSE, glm::value_ptr(m_mvMatrix));

	if (!isInited) {
		std::cout << "please call init() before draw()" << std::endl;
	}

	// draw sphere
	glBindVertexArray(m_vao);
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(GL_PRIMITIVE_RESTART_FIXED_INDEX);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIndex);
	glDrawElements(GL_QUAD_STRIP, numsToDraw, GL_UNSIGNED_INT, NULL);
}

GLuint Sphere::loadShaders(const char *vertFile, const char *fragFile)
{
	// Build and compile our shader program
	// Vertex shader
	std::ifstream in(vertFile);
	std::string contents((std::istreambuf_iterator<char>(in)),
		std::istreambuf_iterator<char>());
	const GLchar * glcontent = contents.c_str();
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &glcontent, NULL);
	glCompileShader(vertexShader);
	// Check for compile time errors
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Fragment shader
	std::ifstream in2(fragFile);
	std::string contents2((std::istreambuf_iterator<char>(in)),
		std::istreambuf_iterator<char>());
	const GLchar * glcontent2 = contents2.c_str();
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &glcontent2, NULL);
	glCompileShader(fragmentShader);
	// Check for compile time errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Link shaders
	mProgramId = glCreateProgram();
	glAttachShader(mProgramId, vertexShader);
	glAttachShader(mProgramId, fragmentShader);
	glLinkProgram(mProgramId);
	// Check for linking errors
	glGetProgramiv(mProgramId, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(mProgramId, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return mProgramId;
}
