#include "Client_Task.h"




client_task::client_task() : ctx_(1),
client_socket_(ctx_, ZMQ_DEALER)
{

}

void client_task::start()
{
	// generate random identity
	char identity[10] = {};
	sprintf(identity, "%04X-%04X", within(0x10000), within(0x10000));
	printf("%s\n", identity);
	client_socket_.setsockopt(ZMQ_IDENTITY, identity, strlen(identity));
	client_socket_.connect("tcp://localhost:5570");

	zmq::pollitem_t items[] = client_socket_, 0, ZMQ_POLLIN, 0;
	int request_nbr = 0;
	try {
		while (true) {
			for (int i = 0; i < 100; ++i) {
				// 10 milliseconds
				zmq::poll(items, 1, 10);
				if (items[0].revents & ZMQ_POLLIN) {
					printf("\n%s ", identity);
					s_dump(client_socket_);
				}
			}
			char request_string[16] = {};
			sprintf(request_string, "request #%d", ++request_nbr);
			client_socket_.send(request_string, strlen(request_string));
		}
	}
	catch (std::exception &e) {}
}
