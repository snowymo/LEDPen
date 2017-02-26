#include "opencv2/opencv.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>
#include <vector>
#include "opencv2/core/matx.hpp"

#include "ImageProcessor.h"

using namespace cv;

char* window_name = "Threshold Demo";

char* trackbar_type = "Type: \n 0: Binary \n 1: Binary Inverted \n 2: Truncate \n 3: To Zero \n 4: To Zero Inverted";
char* trackbar_value = "Value";



/// Function headers
// void Threshold_Demo(int, void*);
// 
// void Threshold_Demo(int, void*)
// {
// 	threshold(gray, gray, threshold_value, max_BINARY_value, threshold_type);
// 	imshow("threshold", gray);
// }

int main() {

	VideoCapture cap(0);
	if (!cap.isOpened())
		return -1;

// 	createTrackbar(trackbar_type,
// 		window_name, &threshold_type,
// 		max_type, Threshold_Demo);
// 	createTrackbar(trackbar_value,
// 		window_name, &threshold_value,
// 		max_value, Threshold_Demo);
	
	/// Create a window to display results
	//namedWindow(window_name, CV_WINDOW_AUTOSIZE);

	ImageProcessor *pImgPro = new ImageProcessor;
	Mat src;

	while (true) {
		cap >> src; // get a new frame from camera
		imshow("src", src);

		// test tutorial file
		//src = imread("Hough_Circle_Tutorial_Theory_0.jpg");

		// set src image
		pImgPro->setSource(src);

		pImgPro->PreProcess();
		
		pImgPro->CheckCircle();
		
		if (waitKey(60)) {
			;
		}
	}

	return 0;
}