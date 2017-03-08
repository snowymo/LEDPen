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

void ZMQPub::send(cv::Vec3f pos)
{
	std::stringstream().swap(_stream);
	_stream.write(reinterpret_cast<const char *>(&pos[0]), 8);
	_stream.write(reinterpret_cast<const char *>(&pos[1]), 8);
	_stream.write(reinterpret_cast<const char *>(&pos[2]), 8);
	s_send(publisher, _stream.str());
}
