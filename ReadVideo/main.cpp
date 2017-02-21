#include "opencv2/opencv.hpp"
#include "opencv2/videoio.hpp"

using namespace cv;

int main(int, char**)
{
	VideoCapture videoFile;
	videoFile.open("sample.mp4");
	if (!videoFile.isOpened())
		return -1;

	double frameCount = videoFile.get(CAP_PROP_FRAME_COUNT);

	//Mat showVideo;
	//namedWindow("player", 1);
	

	for (int i = 0; i < frameCount; i++)
	{
		Mat frame;
		videoFile >> frame; // get a new frame from camera
		
		//cvtColor(frame, edges, COLOR_BGR2GRAY);
		//GaussianBlur(edges, edges, Size(7, 7), 1.5, 1.5);
		//Canny(edges, edges, 0, 30, 3);
		imshow("video", frame);
		std::string st = std::to_string(i);
		//imwrite(st+".jpg", frame);

		if (waitKey(30) >= 0)
//			std::cout << "here!\n";
			;
	}

	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}