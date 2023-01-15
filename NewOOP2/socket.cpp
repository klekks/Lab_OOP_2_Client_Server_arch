#include "socket.h"

Socket::Socket(const std::string& ip, unsigned short port)
{
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip.c_str());

	fd = ::socket(AF_INET, SOCK_STREAM, 0);
}

Socket::Socket(SOCKET sock, sockaddr_in addr) : fd(sock), addr(addr) {}

Socket::Socket(unsigned short port)
{
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);
	addr.sin_family = AF_INET;

	fd = ::socket(AF_INET, SOCK_STREAM, 0);
}

void Socket::set_block_mode()
{
	unsigned long mode = 0;
	ioctlsocket(fd, FIONBIO, &mode);
}

void Socket::set_nonblock_mode()
{
	unsigned long mode = 1;
	ioctlsocket(fd, FIONBIO, &mode);
}

void Socket::bind()
{
	int res = ::bind(fd, (sockaddr*)&addr, sizeof(addr));
	if (res == SOCKET_ERROR)
		throw std::exception("Binding error");
}

void Socket::listen(int queue_size)
{
	::listen(fd, queue_size);
}

void Socket::select_with(WSAEVENT& event, long mask)
{
	WSAEventSelect(fd, event, mask);
}

Socket Socket::accept()
{
	sockaddr_in addr;
	int socksize = sizeof(addr);
	SOCKET client_sock = ::accept(fd, (sockaddr*)&addr, &socksize);

	return Socket(client_sock, addr);
}

SOCKET Socket::socket()
{
	return fd;
}