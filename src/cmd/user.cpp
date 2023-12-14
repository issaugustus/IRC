#include "Numeric.hpp"
#include "Client.class.hpp"
#include "Channel.class.hpp"
#include "Server.class.hpp"
#include <iostream>
#include <sstream>

int	Client::user(const Message & msg)
{
	bool	erroneus;

	if (msg.getCommand() != "USER")
		return (1);
	else if ((int) msg.getParamSize() < 4)
	{
		this->sendReply(ERR_NEEDMOREPARAMS, Error::needmoreparams(msg.getCommand()));
		return (-1);
	}
	else if (!_hasPassword)
	{
		this->sendReply(ERR_PASSWDMISSMATCH, Error::passwdmissmatch());
		return (-1);
	}
	else if (_isRegistered)
	{
		this->sendReply(ERR_ALREADYREGISTRED, Error::alreadyregistered());
		return (-1);
	}
	else
	{
		_username = msg.getParam(0);
		erroneus = !isalnum(msg.getParam(0).at(0));
		for (int i = 1; i < (int) msg.getParam(0).size(); i++)
		{
			if (i + 1 < (int) msg.getParam(0).size() && msg.getParam(0).at(i) == '-')
			{
				erroneus = true;
				sendReply(ERR_ERRONEUSNICKNAME, Error::erroneusnickname(msg.getParam(0)));
			}	
			else if (msg.getParam(0).at(i) != '-' && !isalnum(msg.getParam(0).at(i)))
			{
				erroneus = true;
				sendReply(ERR_ERRONEUSNICKNAME, Error::erroneusnickname(msg.getParam(0)));
			}
		}
		_hostname = msg.getParam(1);
		if (msg.getParam(2) != "*")
			_servername = msg.getParam(2);
		else
			_servername = _server->getName();
		if (!erroneus)
			erroneus = !isalnum(msg.getParam(3).at(0));
		for (int i = 1; i < (int) msg.getParam(3).size(); i++)
		{
			if (i + 1 < (int) msg.getParam(3).size() && msg.getParam(3).at(i) == '-')
			{
				erroneus = true;
				sendReply(ERR_ERRONEUSNICKNAME, Error::erroneusnickname(msg.getParam(3)));
			}
			else if (msg.getParam(3).at(i) != '-' && !isalnum(msg.getParam(3).at(i)))
			{
				erroneus = true;
				sendReply(ERR_ERRONEUSNICKNAME, Error::erroneusnickname(msg.getParam(3)));
			}
		}
		_name = msg.getParam(3);
		if (erroneus)
			return (-1);
		_hasUsername = true;
		if (_hasNickname)
		{
			_isRegistered = true;
			this->send_buff(":" + _server->getName() + " 001 " + _nickname + " :Connection to server established");
			this->sendReply(RPL_WELCOME, Reply::welcome(_username));
		}
		return (0);
	}
	return (-1);
}