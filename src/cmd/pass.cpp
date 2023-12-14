#include "Numeric.hpp"
#include "Client.class.hpp"
#include "Channel.class.hpp"
#include "Server.class.hpp"
#include "ErrorCode.hpp"
#include <iostream>
#include <sstream>

int	Client::pass(const Message &msg)
{
	if (msg.getCommand() != "PASS")
		return (1);
	else if ((int) msg.getParamSize() < 1)
	{
		this->sendReply(ERR_NEEDMOREPARAMS, Error::needmoreparams(msg.getCommand()));
		return (-1);
	}
	else if (_isRegistered)
	{
		this->sendReply(ERR_ALREADYREGISTRED, Error::alreadyregistered());
		return (-1);
	}
	else
	{
		_password = msg.getParam(0);
		if (_password == _server->getPassword())
			_hasPassword = true;
		else
		{
			this->sendReply(ERR_PASSWDMISSMATCH, Error::passwdmissmatch());
			if (DEBUG)
				std::cerr << "password : " << _password << " expected : " << _server->getPassword() << std::endl;
			return (-1);
		}
	}
	return (0);
}