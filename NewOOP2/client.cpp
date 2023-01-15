#include "client.h"
#include "message.h"
#include <vector>
#include <iostream>
#include <fstream>

Client::Client(const Socket& sock) : sock(sock){}

SOCKET Client::socket()
{
	return sock.socket();
}

void Client::select_with(WSAEVENT& event)
{
	WSAEventSelect(socket(), event, FD_ACCEPT | FD_READ | FD_WRITE);
}

bool Client::recive_message()
{
	char buff[256];

	int len = ::recv(socket(), buff, 256, 0);

	if (len < 0)
		throw std::exception("No data");

	if (buff[0] = 'p' && buff[1] == 'u' && buff[2] == 't')
	{
		memcpy(buff, buff + 3, 253);
		len -= 3;
	}

	do
	{
		input_buffer.insert(input_buffer.end(), buff, buff + len);
		len = ::recv(socket(), buff, 256, 0);
	} while (len > 0);
	
	Message msg;
	bool stop_arrived = false;
	std::ofstream file("msg.txt", std::ios_base::app);
	while (msg.is_full_message(input_buffer))
	{
		msg.extract_message(input_buffer);
		file << msg;
		if (msg.is_stop_message())
			stop_arrived = true;
		output_buffer.insert(output_buffer.end(), { 'o', 'k' });
	}
	file.close();
	return !stop_arrived;
}

void Client::send_message()
{
	if (output_buffer.empty())
		return;

	int size = send(socket(), output_buffer.data(), output_buffer.size(), 0);
	output_buffer.erase(output_buffer.begin(), output_buffer.begin() + size);
}

bool Client::ready_to_send()
{
	return !output_buffer.empty();
}


void Client::destroy()
{
	send_message();
}