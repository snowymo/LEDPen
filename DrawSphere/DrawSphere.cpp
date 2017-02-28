#define _CRT_SECURE_NO_WARNINGS
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include "OGLTriangle.h"
#include "OGLSphere.h"
#include <iostream>
#include "OGLSquare.h"
#include "opencv2/opencv.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/core/matx.hpp"
#include "ImageProcessor.h"



int width, height;
OGLTriangle * pTri = new OGLTriangle;
//OGLSphere * pSphere = new OGLSphere;
OGLSquare * pSq = new OGLSquare;
float x = 11, y = -15;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// When a user presses the escape key, we set the WindowShouldClose property to true, 
	// closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	else if (key == GLFW_KEY_W) {
		y += 2;
	}
	else if (key == GLFW_KEY_S) {
		y -= 2;
	}
	else if (key == GLFW_KEY_A) {
		x -= 2;
	}
	else if (key == GLFW_KEY_D) {
		x += 2;
	}
	//astd::cout << x << "\t" << y << "\n";
}

void display() {
	// wipe the drawing surface clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glUseProgram(shader_programme);
	//drawTriangle();
	//pTri->draw();
	// drwa sphere
	//pSphere->draw();
	//ptrSphere->draw();
	pSq->draw(glm::vec3(1,0,0));
}


int main() {
	cv::VideoCapture cap(0);
	if (!cap.isOpened())
		return -1;

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

	ImageProcessor *pImgPro = new ImageProcessor;
	cv::Mat src;
	std::vector<cv::Vec3f> circles;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		// update other events like input handling 
		glfwPollEvents();

		// Clear the color buffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		// wipe the drawing surface clear
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// deal with opencv
		cap >> src; // get a new frame from camera
		cv::imshow("src", src);
		// set src image
		pImgPro->setSource(src);
		pImgPro->PreProcess();
		pImgPro->CheckCircle();
		
		circles = pImgPro->getCircles();
		for (int i = 0; i < circles.size(); i++) {
			// calculate world position
			// Rvec-1 * ( (intrisic)-1 * s * [u,v,1] - tvec)
			//cv::Mat rtvec = pImgPro->getRTVec();// rtvec
			//cv::Matx31f rvec(rtvec.at<double>(0, 0), rtvec.at<double>(0, 1), rtvec.at<double>(0, 2));
			//cv::Matx33f rmat;
			//cv::Rodrigues(rvec, rmat);
			//cv::Matx31f tvec(rtvec.at<double>(0, 3), rtvec.at<double>(0, 4), rtvec.at<double>(0, 5));
			//cv::Matx33f intrisic(pImgPro->getIntrisic());
			//cv::Matx31f screen(circles[i][0]-320,240-circles[i][1],1);
			//float sc = 0.1;
			//cv::Matx31f worldpos = rmat.inv() * (intrisic.inv() * sc * screen - tvec);
			//std::cout << "\tworld:" << worldpos << "\n";
// 			sc = 0.01;
// 			worldpos = rmat.inv() * (intrisic.inv() * sc * screen - tvec);
// 			std::cout << "\tworld:" << worldpos << "\n";
			// Render
			pSq->draw(glm::vec3(circles[i][0] / 640.0f, circles[i][1] / 480.0f, 0), 1/*circles[i][2]*/);
		}
		pSq->draw(glm::vec3(x,3,y), 5);
		
		//display();
		// put the stuff we've been drawing onto the display
		glfwSwapBuffers(window);
	}

	// close GL context and any other GLFW resources
	glfwTerminate();
	return 0;
}
