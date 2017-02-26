#include "ImageProcessor.h"
#include "opencv2\imgproc.hpp"
#include "opencv2\highgui.hpp"
#include <iostream>
#include <time.h>
#include <chrono>
#include "opencv2\core\persistence.hpp"

ImageProcessor::ImageProcessor()
{

}

ImageProcessor::ImageProcessor(cv::Mat)
{

}

ImageProcessor::~ImageProcessor()
{

}

void ImageProcessor::setSource(cv::Mat ref)
{
	// shallow copy
	mSource = ref;
}

void ImageProcessor::PreProcess()
{
	undistort();
	turnGray();
	//threshold();
	blur();
}

std::chrono::time_point<std::chrono::system_clock> start, end;
std::chrono::duration<double> elapsed;

void ImageProcessor::CheckCircle()
{
	/// Apply the Hough Transform to find the circles
	
	start = std::chrono::system_clock::now();
	HoughCircles(mBlur, mCircles, CV_HOUGH_GRADIENT, 1/*1.3*/, mBlur.rows / 16, 200, 55, 0, 0);
	end = std::chrono::system_clock::now();
	elapsed = end - start;
	std::cout << "amount of circle " << mCircles.size() << " after " << elapsed.count() << "s\n";

	/// Draw the circles detected
	for (size_t i = 0; i < mCircles.size(); i++)
	{
		cv::Point center(cvRound(mCircles[i][0]), cvRound(mCircles[i][1]));
		int radius = cvRound(mCircles[i][2]);
		// circle center
		circle(mSource, center, 3, cv::Scalar(0, 255, 0), -1, 8, 0);
		// circle outline
		circle(mSource, center, radius, cv::Scalar(255, 0, 255), 3, 8, 0);
	}
	/// Show your results
	cv::imshow("result", mSource);

}

void ImageProcessor::CalculateSpheres()
{
	// based on circle's [x,y], calculate spheres [x,y,z] with config

}

void ImageProcessor::turnGray()
{
	cv::cvtColor(mSource, mGray, cv::COLOR_BGR2GRAY);
	//cv::imshow("gray", mGray);
}

void ImageProcessor::threshold()
{
	cv::threshold(mGray, mThres, threshold_value, max_BINARY_value, threshold_type);
	cv::imshow("threshold", mGray);
}

void ImageProcessor::blur()
{
	/// Reduce the noise so we avoid false circle detection
	cv::GaussianBlur(mGray, mBlur, cv::Size(9, 9), 2, 2);
}

void ImageProcessor::undistort()
{
	mUndistort = mSource.clone();
	cv::undistort(mUndistort, mSource, cameraMatrix, distCoeffs);

}

std::string cmrcfg = "cam2.xml";
void ImageProcessor::importConfig()
{
	// read camera matrix from file
	cv::FileStorage fs(cmrcfg, cv::FileStorage::READ);
	if (cameraMatrix.empty()) {
		fs["camera_matrix"] >> cameraMatrix;
		fs["distortion_coefficients"] >> distCoeffs;
	}
}
