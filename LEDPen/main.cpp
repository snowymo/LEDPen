#include "opencv2/opencv.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>
#include <vector>
#include "opencv2/core/matx.hpp"

using namespace cv;

char* window_name = "Threshold Demo";

char* trackbar_type = "Type: \n 0: Binary \n 1: Binary Inverted \n 2: Truncate \n 3: To Zero \n 4: To Zero Inverted";
char* trackbar_value = "Value";
int threshold_type = 3;;
int const max_value = 255;
int const max_type = 4;
int const max_BINARY_value = 255;
int threshold_value = 107;

Mat src, gray;

/// Function headers
void Threshold_Demo(int, void*);

void Threshold_Demo(int, void*)
{
	threshold(gray, gray, threshold_value, max_BINARY_value, threshold_type);
	imshow("threshold", gray);
}

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

	while (true) {
		cap >> src; // get a new frame from camera
		imshow("src", src);

		// test tutorial file
		//src = imread("Hough_Circle_Tutorial_Theory_0.jpg");

		cvtColor(src, gray, COLOR_BGR2GRAY);
		imshow("gray", gray);

		/// Call the function to initialize
		//Threshold_Demo(0, 0);
// 		threshold(gray, gray, threshold_value, max_BINARY_value, threshold_type);
// 		imshow("threshold", gray);

		/// Reduce the noise so we avoid false circle detection
		GaussianBlur(gray, gray, Size(9, 9), 2, 2);

		
		std::vector<Vec3f> circles;

		/// Apply the Hough Transform to find the circles
		HoughCircles(gray, circles, CV_HOUGH_GRADIENT, 1.25, gray.rows / 16, 200, 55, 0, 0);
		std::cout << "amount of circle " << circles.size() << "\n";

		/// Draw the circles detected
		for (size_t i = 0; i < circles.size(); i++)
		{
			Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
			int radius = cvRound(circles[i][2]);
			// circle center
			circle(src, center, 3, Scalar(0, 255, 0), -1, 8, 0);
			// circle outline
			circle(src, center, radius, Scalar(255, 0, 255), 3, 8, 0);
		}

		/// Show your results
		imshow("result", src);

		if (waitKey(60)) {
			;
		}
	}

	return 0;
}