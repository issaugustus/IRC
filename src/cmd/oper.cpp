#include "Numeric.hpp"
#include "Client.class.hpp"
#include "Channel.class.hpp"
#include "Server.class.hpp"
#include <iostream>
#include <sstream>
#include <stdlib.h>

int	Client::oper(const Message & msg)
{
	if ((int) msg.getParamSize() < 2)
	{
		sendReply(ERR_NEEDMOREPARAMS, Error::needmoreparams(msg.getCommand()));
		return (-1);
	}
	if (this->_isOperator)
	{
		sendReply(RPL_YOUREOPER, Reply::youreoper());
		return (0);
	}
	if (msg.getParam(1) != OPER_PASSWORD)
	{
		sendReply(ERR_PASSWDMISSMATCH, Error::passwdmissmatch());
		return (-1);
	}
	this->_isOperator = true;
	sendReply(RPL_YOUREOPER, Reply::youreoper());
	return (0);
}