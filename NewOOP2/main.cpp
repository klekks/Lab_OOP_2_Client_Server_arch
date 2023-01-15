#include "server.h"

int main()
{
	WSADATA unused;
	WSAStartup(MAKEWORD(2, 2), &unused);

	Server server(9000);
	server.start();
	server.wsa_loop();
}