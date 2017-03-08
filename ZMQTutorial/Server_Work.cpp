#include "Server_Work.h"



void server_worker::work()
{
	worker_.connect("inproc://backend");

	try {
		while (true) {
			zmq::message_t identity;
			zmq::message_t msg;
			zmq::message_t copied_id;
			zmq::message_t copied_msg;
			worker_.recv(&identity);
			worker_.recv(&msg);

			int replies = within(5);
			for (int reply = 0; reply < replies; ++reply) {
				s_sleep(within(1000) + 1);
				copied_id.copy(&identity);
				copied_msg.copy(&msg);
				worker_.send(copied_id, ZMQ_SNDMORE);
				worker_.send(copied_msg);
			}
		}
	}
	catch (std::exception &e) {}
}
