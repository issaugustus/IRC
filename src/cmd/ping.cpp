#include "Numeric.hpp"
#include "Client.class.hpp"
#include "Channel.class.hpp"
#include "Server.class.hpp"
#include "Message.class.hpp"
#include "ErrorCode.hpp"
#include <iostream>
#include <sstream>

int	Client::ping(const Message & msg)
{
	std::string	response = "PONG :";
	if (msg.getCommand() != "PING")
		return (1);
	else if (msg.getParamSize() > (size_t) 0)
		response += msg.getParam(0);
	else
		response += "hey " + this->getNickname();
	this->_lastActivity = std::time(0);
	this->_isUp = true;
	this->send_buff(response);
	return (0);
}
