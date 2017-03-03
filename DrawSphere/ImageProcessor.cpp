#include "ImageProcessor.h"
#include "opencv2\imgproc.hpp"
#include "opencv2\highgui.hpp"
#include <iostream>
#include <time.h>
#include <chrono>
#include "opencv2\core\persistence.hpp"
#include "opencv2\photo.hpp"
#include <fstream>
#include "opencv2\imgcodecs.hpp"

ImageProcessor::ImageProcessor()
{
	circleNum = 2;
	lifeTime = 0;
	maxLifeTime = 10;
	maxDistance = 350;

	dp = 1.1;
	p1 = 205;
	p2 = 20;
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
	//underexposure();
}

std::chrono::time_point<std::chrono::system_clock> start, end;
std::chrono::duration<double> elapsed;

void ImageProcessor::CheckCircle()
{
	/// Apply the Hough Transform to find the circles
	start = std::chrono::system_clock::now();
	HoughCircles(mBlur, mAllCircles, CV_HOUGH_GRADIENT, dp, mBlur.rows / 16, p1, p2, 0, 0);
	end = std::chrono::system_clock::now();
	elapsed = end - start;
	//std::cout << "amount of circle " << mAllCircles.size() << " after " << elapsed.count() << "s\n";

	/// Draw the circles detected
	for (size_t i = 0; i < mAllCircles.size(); i++)
	{
		cv::Point center(cvRound(mAllCircles[i][0]), cvRound(mAllCircles[i][1]));
		int radius = cvRound(mAllCircles[i][2]);
		// circle center
		circle(mSource, center, 3, cv::Scalar(0, 120, 0), -1, 8, 0);
		// circle outline
		circle(mSource, center, radius, cv::Scalar(120, 255, 120), 3, 8, 0);
		//std::cout << "\tall circle:" << mAllCircles[i] << "\n";
	}

	track();
	
	for (size_t i = 0; i < mCircles.size(); i++)
	{
		cv::Point center(cvRound(mCircles[i][0]), cvRound(mCircles[i][1]));
		int radius = cvRound(mCircles[i][2]);
		// circle center
		circle(mSource, center, 3, cv::Scalar(0, 255, 0), -1, 8, 0);
		// circle outline
		circle(mSource, center, radius, cv::Scalar(255, 0, 255), 3, 8, 0);
		//std::cout << "\tcircle:" << mCircles[i] << "\n";
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
	importConfig();
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
		fs["extrinsic_parameters"] >> rtvec;
	}
}

void ImageProcessor::track()
{
	// if no previous circle
	if (mCircles.size() == 0) {
		if (mAllCircles.size() <= circleNum) {
			// keep all of them
			mCircles = mAllCircles;
		}
		return;
	}

	// only certain number of circles could be detected
	++lifeTime;
	std::vector<cv::Vec3f> mCurCircle;
	while( !mCircles.empty()){
	//for (int i = 0; i < mCircles.size(); i++) {
		float minDis = maxDistance;
		int minIdx = -1;
		for( int j = 0; j < mAllCircles.size(); j++) {
			// if there is previous circle, check the distance
			float dis = cv::norm(cv::Vec3f(mAllCircles[j][0],mAllCircles[j][1]) - cv::Vec3f(mCircles.back()[0],mCircles.back()[1]), cv::NORM_L2);
			//std::cout << "test dis " << dis << "\n";
			// the position of the circle should be close enough to previous one
			if (dis < maxDistance) {
				if (dis < minDis) {
					minDis = dis;
					minIdx = j;
				}
			}
		}
		// allCircle[j] should be kept
		if (minIdx != -1) {
			mCurCircle.push_back(mAllCircles[minIdx]);
			mAllCircles.erase(mAllCircles.begin() + minIdx);
			lifeTime = 0;
		}
		else if (lifeTime < maxLifeTime) {
			mCurCircle.push_back(mCircles.back());
		}
		else {
			lifeTime = 0;
		}
		mCircles.pop_back();
	}

	if (mCurCircle.size() > 1) {
		float dis = cv::norm(cv::Vec3f(mCurCircle[0][0], mCurCircle[0][1]) - cv::Vec3f(mCurCircle[1][0], mCurCircle[1][1]), cv::NORM_L2);
		std::cout << "two close circle dis:\t" << dis << "\n";
		if (dis <= 40) {
			mCurCircle.pop_back();
		}
	}

	mCircles = mCurCircle;
	while (mCircles.size() < 2 && mAllCircles.size() > 0) {
		float dis = cv::norm(cv::Vec3f(mCurCircle[0][0],mCurCircle[0][1]) - cv::Vec3f(mAllCircles.back()[0],mAllCircles.back()[1]), cv::NORM_L2);
		std::cout << "two close circle dis:\t" << dis << "\n";
		if (dis > 40) {
			mCircles.push_back(mAllCircles.back());
		}
		mAllCircles.pop_back();
	}
}

void ImageProcessor::readImages(std::string path, std::vector<cv::Mat>& images, std::vector<float>& times)
{
	path = path + std::string("/");
	std::ifstream list_file((path + "list.txt").c_str());
	std::string name;
	float val;
	while (list_file >> name >> val) {
		cv::Mat img = cv::imread(path + name);
		images.push_back(img);
		times.push_back(1 / val);
	}
	list_file.close();
}

void ImageProcessor::underexposure()
{
	std::vector<cv::Mat> images;
	std::vector<float> times;
	readImages("exposureImg", images, times);

	cv::Mat response;
	cv::Ptr<cv::CalibrateCRF> debevecCalib = cv::createCalibrateDebevec();

	cv::Mat hdr;
	cv::Ptr<cv::MergeDebevec> merge_debevec = cv::createMergeDebevec();
	merge_debevec->process(images, hdr, times, response);
}
