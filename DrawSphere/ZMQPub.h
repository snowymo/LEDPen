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
	void send(cv::Vec3f);

private:
	zmq::context_t context;
	zmq::socket_t publisher;
	std::stringstream _stream;
};

