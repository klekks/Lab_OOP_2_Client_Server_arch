#include "server.h"

#include <iostream>

Server::Server(unsigned short port) : sock(port)
{
	sock.set_nonblock_mode();
	events[0] = WSACreateEvent();
	events[1] = WSACreateEvent();
}

void Server::start()
{
	sock.bind();
	sock.listen(256);
}

void Server::accept()
{
	Socket client_sock = sock.accept();
	Client client(client_sock);
	client.select_with(events[1]);
	clients.push_back(client);
}

void Server::wsa_loop()
{
	sock.select_with(events[1], FD_ACCEPT);

	bool state_run = true;
	
	while (state_run)
	{
		WSANETWORKEVENTS ne;
		DWORD dw = WSAWaitForMultipleEvents(2, events, FALSE, 1000, FALSE);
		WSAResetEvent(events[0]);
		WSAResetEvent(events[1]);

		for (auto &client: clients)
		{
			if (0 == WSAEnumNetworkEvents(client.socket(), events[1], &ne))
			{
				if (ne.lNetworkEvents & FD_READ)
				{
					state_run = client.recive_message();
				}
				if (ne.lNetworkEvents & FD_CLOSE)
				{
					client.destroy();
				}

				if (ne.lNetworkEvents & FD_WRITE || client.ready_to_send())
				{
					client.send_message();
				}
			}

		}

		if (WSAEnumNetworkEvents(sock.socket(), events[0], &ne) == 0 && (ne.lNetworkEvents & FD_ACCEPT))
		{
			std::cout << "connected" << std::endl;
			accept();
		}
	}
}
