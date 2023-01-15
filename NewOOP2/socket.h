#pragma once
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <windows.h>
#include <winsock2.h>

#include <string>

#pragma comment(lib, "ws2_32.lib") 


class Socket
{
public:
	Socket(const std::string& ip, unsigned short port);
	Socket(const Socket&) = default;
	Socket(SOCKET sock, sockaddr_in addr);
	Socket(unsigned short port);

	void set_block_mode();
	void set_nonblock_mode();

	void bind();
	void listen(int);

	void select_with(WSAEVENT& event, long mask);

	Socket accept();
	void connect();

	SOCKET socket();

private:
	SOCKET fd;
	sockaddr_in addr;
};
