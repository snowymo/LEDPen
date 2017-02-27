#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include "OGLTriangle.h"
#include "OGLSphere.h"
#include <iostream>
#include "OGLSquare.h"
int width, height;
OGLTriangle * pTri = new OGLTriangle;
//OGLSphere * pSphere = new OGLSphere;
OGLSquare * pSq = new OGLSquare;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// When a user presses the escape key, we set the WindowShouldClose property to true, 
	// closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void display() {
	// wipe the drawing surface clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glUseProgram(shader_programme);
	//drawTriangle();
	pTri->draw();
	// drwa sphere
	//pSphere->draw();
	//ptrSphere->draw();
	pSq->draw(glm::vec3(1,0,0));
}


int main() {
	// start GL context and O/S window using the GLFW helper library
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(640, 480, "Circle Detection", NULL, NULL);
	if (!window) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);

	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"


	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	pTri->LinkShaders();
	//pSphere->LinkShaders();
	pSq->LinkShaders();
	
	/* OTHER STUFF GOES HERE NEXT */
	//pTri->create();
	//pSphere->create();
	pSq->create();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		// update other events like input handling 
		glfwPollEvents();
		// Render
		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		display();
		// put the stuff we've been drawing onto the display
		glfwSwapBuffers(window);
	}

	// close GL context and any other GLFW resources
	glfwTerminate();
	return 0;
}
