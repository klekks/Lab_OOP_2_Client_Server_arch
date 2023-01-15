#pragma once
#include "socket.h"
#include <vector>

class Client
{
public:
	Client(const Socket& sock);

	void select_with(WSAEVENT& event);
	SOCKET socket();

	bool recive_message();
	void send_message();
	void destroy();
	bool ready_to_send();
private:
	Socket sock;
	std::vector<char> input_buffer;
	std::vector<char> output_buffer;
};