#include "Numeric.hpp"
#include "Client.class.hpp"
#include "Channel.class.hpp"
#include "Server.class.hpp"
#include "Message.class.hpp"
#include "ErrorCode.hpp"
#include <iostream>
#include <sstream>

int	Client::pong(const Message & msg)
{
	if (msg.getParamSize() < (size_t) 1)
	{
		sendReply(ERR_NEEDMOREPARAMS, Error::needmoreparams(msg.getCommand()));
		return (-1);
	}
	this->_lastActivity = std::time(0);
	this->_isUp = true;
	return (0);
}
