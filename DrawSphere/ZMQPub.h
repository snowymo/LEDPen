#include "zhelpers.hpp"
#include <string>
#include "opencv2/opencv.hpp"
#include "opencv2/core/matx.hpp"
#include <sstream>

class ZMQPub
{
public:
	ZMQPub();
	~ZMQPub();
	void send(std::string st);
	void send(cv::Vec3f, std::chrono::milliseconds ms2 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()));

private:
	zmq::context_t context;
	zmq::socket_t publisher;
	std::stringstream _stream;
};

