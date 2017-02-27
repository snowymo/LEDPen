#ifndef _IMAGE_PROCESSOR_H
#define _IMAGE_PROCESSOR_H

#include "opencv2/calib3d.hpp"

class ImageProcessor {
public:
	ImageProcessor();
	ImageProcessor(cv::Mat);
	~ImageProcessor();

	std::vector<cv::Vec3f> getCircles() const { return mCircles; }
	
public:
	void setSource(cv::Mat);

public:
	void PreProcess();
	void CheckCircle();
	void CalculateSpheres();

private:
	void turnGray();
	void threshold();
	void blur();
	void undistort();
	void importConfig();

private:
	cv::Mat mSource, mGray, mThres, mBlur, mUndistort;
	int threshold_type = 3;;
	int const max_value = 255;
	int const max_type = 4;
	int const max_BINARY_value = 255;
	int threshold_value = 107;

	// circle information
	std::vector<cv::Vec3f> mCircles;	//[0]:x, [1]:y, [2]:r
	cv::Mat cameraMatrix, distCoeffs, extrinsicMatrix, rtvec;

public:
	cv::Mat getRTVec() { return rtvec; }
	cv::Mat getIntrisic() { return cameraMatrix; }

};

#endif // _IMAGE_PROCESSOR_H
