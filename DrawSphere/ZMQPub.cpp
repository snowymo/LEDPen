#include "ZMQPub.h"



ZMQPub::ZMQPub() :context(1),
publisher(context, ZMQ_PUB)
{
	//zmq::context_t context(1);
	//  We send updates via this socket
	//publisher =  zmq::socket_t(context, ZMQ_PUB);
	publisher.bind("tcp://192.168.1.44:5565");
}


ZMQPub::~ZMQPub()
{
}

void ZMQPub::send(std::string st)
{
	s_send(publisher, st);
}

void ZMQPub::send(cv::Vec3f pos, std::chrono::milliseconds ms2)
{
	std::stringstream().swap(_stream);
	cv::Vec3f unityPos = cv::Vec3f(pos[1] * sqrtf(2) / 2 - pos[2] * sqrtf(2) / 2, pos[2], pos[0]);
	unityPos[0] = (unityPos[0] / 35 - 5) * 0.15;
	unityPos[1] = (unityPos[1] / 50 - 1) * 0.15 + 1.0;
	unityPos[2] = (unityPos[2] / 140 - 3) * 0.152;
	_stream.write(reinterpret_cast<const char *>(&unityPos[0]), 4);
	_stream.write(reinterpret_cast<const char *>(&unityPos[1]), 4);
	_stream.write(reinterpret_cast<const char *>(&unityPos[2]), 4);

	//std::chrono::milliseconds ms2 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
 	int t = ms2.count() % (1000 * 60 * 60);
// 	std::cout << "t:" << t << "\n";
	_stream.write(reinterpret_cast<const char *>(&t), 4);

	s_send(publisher, _stream.str());
}
