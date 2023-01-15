#pragma once

#include <string>
#include <vector>

class Message
{
public:
	Message();
	bool is_full_message(const std::vector<char>&);
	void extract_message(std::vector<char>&);
	friend std::ostream& operator<<(std::ostream&, const Message&);
	bool is_stop_message();

private:
	char h1, h2, m1, m2, s1, s2;
	unsigned int B;
	int length;
	std::string text;
};
