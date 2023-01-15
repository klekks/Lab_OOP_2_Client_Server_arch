#include "message.h"

#include "socket.h"
#include <fstream>
#include <iostream>

Message::Message()
{
	h1 = h2 = m1 = m2 = s1 = s2 = B = length = 0;
}

bool Message::is_full_message(const std::vector<char>& buffer)
{
	if (buffer.size() < 14) return false;

	for (int i = 14; i < buffer.size(); i++)
		if (buffer[i] == 0)
		{
			length = i;
			return true;
		}

	return false;
}

void Message::extract_message(std::vector<char>& buffer)
{
	text.clear();

	h1 = buffer[4]; m1 = buffer[5]; s1 = buffer[6];
	h2 = buffer[7]; m2 = buffer[8]; s1 = buffer[9];

	B = ntohl(*(unsigned*)&buffer[10]);

	text.append(buffer.begin() + 14, buffer.begin() + length);

	buffer.erase(buffer.begin(), buffer.begin() + length + 1);
}

bool Message::is_stop_message()
{
	return text == "stop";
}


std::ostream& operator<<(std::ostream& output, const Message& msg)
{
	output << int(msg.h1) << ":" << int(msg.m1) << ":" << int(msg.s1) << " " << int(msg.h2) << ":" << int(msg.m2) << ":" << int(msg.s2) << " " << int(msg.B) << " " << msg.text << std::endl;
	return output;
}