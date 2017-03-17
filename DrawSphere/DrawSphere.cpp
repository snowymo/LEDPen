#define _CRT_SECURE_NO_WARNINGS
//#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include "opencv2/opencv.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/core/matx.hpp"
//#include "OGLTriangle.h"
//#include "OGLSphere.h"
#include <iostream>
//#include "OGLSquare.h"

#include "ImageProcessor.h"
#include "ZMQPub.h"


int width, height;
//OGLTriangle * pTri = new OGLTriangle;
//OGLSphere * pSphere = new OGLSphere;
//OGLSquare * pSq = new OGLSquare;
float x = 0, y = 0;
cv::Mat src;
ImageProcessor *pImgPro = new ImageProcessor;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// When a user presses the escape key, we set the WindowShouldClose property to true, 
	// closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	else if (key == GLFW_KEY_W) {
		y += 1;
	}
	else if (key == GLFW_KEY_S) {
		y -= 1;
		//cv::imwrite("test.png", src);
	}
	else if (key == GLFW_KEY_A) {
		x -= 2;
	}
	else if (key == GLFW_KEY_D) {
		x += 2;
	}
	else if (key == GLFW_KEY_U) {
		pImgPro->dp += 0.05;
		std::cout << "dp:" << pImgPro->dp << "\n";
	}
	else if (key == GLFW_KEY_J) {
		pImgPro->dp -= 0.05;
		std::cout << "dp:" << pImgPro->dp << "\n";
	}
	else if (key == GLFW_KEY_I) {
		pImgPro->p1 += 5;
		std::cout << "p1:" << pImgPro->p1 << "\n";
	}
	else if (key == GLFW_KEY_K) {
		pImgPro->p1 -= 5;
		std::cout << "p1:" << pImgPro->p1 << "\n";
	}
	else if (key == GLFW_KEY_O) {
		pImgPro->p2 += 3;
		std::cout << "p2:" << pImgPro->p2 << "\n";
	}
	else if (key == GLFW_KEY_L) {
		pImgPro->p2 -= 3;
		std::cout << "p2:" << pImgPro->p2 << "\n";
	}
}


int main() {
	cv::VideoCapture cap(0);
	if (!cap.isOpened())
		return -1;
	bool success = cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
	success = cap.set(cv::CAP_PROP_FRAME_HEIGHT, 360);
	success = cap.set(cv::CAP_PROP_FPS, 60);
	int curexposure = cap.get(CV_CAP_PROP_EXPOSURE);
	int curAutoExpo = cap.get(CV_CAP_PROP_AUTO_EXPOSURE);
	success = cap.set(CV_CAP_PROP_EXPOSURE, -8);
	//bool success = cap.set(CV_CAP_PROP_AUTO_EXPOSURE, 2);
	std::cout << success;
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
// 	glewExperimental = GL_TRUE;
// 	glewInit();

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

	//pTri->LinkShaders();
	//pSphere->LinkShaders();
	//pSq->LinkShaders();
	
	/* OTHER STUFF GOES HERE NEXT */
	//pTri->create();
	//pSphere->create();
	//pSq->create();

	
	
	std::vector<cv::Vec3f> circles;
	ZMQPub *zp = new ZMQPub;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		// update other events like input handling 
		glfwPollEvents();

		std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());

		// deal with opencv
		cap >> src; // get a new frame from camera
		//cv::imshow("src", src);
		// set src image
		
		//std::chrono::seconds s = std::chrono::duration_cast<std::chrono::seconds>(ms);
		pImgPro->setSource(src);
		pImgPro->PreProcess();
		pImgPro->CheckCircle();
		
		circles = pImgPro->getCircles();
		std::cout << "pos:" << pImgPro->getPos() << "\n";
		std::chrono::milliseconds ms2 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
		std::cout << (ms2 - ms).count() << "\n";
		zp->send(pImgPro->getPos(),ms);

		// put the stuff we've been drawing onto the display
		glfwSwapBuffers(window);
	}

	// close GL context and any other GLFW resources
	glfwTerminate();
	return 0;
}
