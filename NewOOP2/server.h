#pragma once

#include <vector>

#include "socket.h"
#include "client.h"

class Server
{
public:
	Server(unsigned short port);
	void start();
	void wsa_loop();
	void accept();
private:
	Socket sock;
	std::vector<Client> clients;
	WSAEVENT events[2];
};