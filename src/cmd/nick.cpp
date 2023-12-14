#include "Numeric.hpp"
#include "Client.class.hpp"
#include "Channel.class.hpp"
#include "Server.class.hpp"
#include <iostream>
#include <sstream>

int	Client::nick(const Message & msg)
{
	bool	erroneus;
	bool	inUse;

	if (msg.getCommand() != "NICK")
		return (1);
	else if ((int) msg.getParamSize() < 1)
	{
		this->sendReply(ERR_NONICKNAMEGIVEN, Error::nonicknamegiven());
		return (-1);
	}
	else if (!_hasPassword)
	{
		this->sendReply(ERR_PASSWDMISSMATCH, Error::passwdmissmatch());
		return (-1);
	}
	else
	{
		erroneus = !isalnum(msg.getParam(0).at(0));
		for (int i = 1; i < (int) msg.getParam(0).size(); i++)
		{
			if (i + 1 < (int) msg.getParam(0).size() && msg.getParam(0).at(i) == '-')
				erroneus = true;
			else if (msg.getParam(0).at(i) != '-' && !isalnum(msg.getParam(0).at(i)))
				erroneus = true;
		}
		if (erroneus)
		{
			this->sendReply(ERR_ERRONEUSNICKNAME, Error::erroneusnickname(msg.getParam(0)));
			return (-1);
		}
		else
		{
			inUse = false;
			for (int i = 0; i < (int) _server->getClientsSize(); i++)
			{
				if (_server->getClient(i)->getNickname() == msg.getParam(0) && _server->getClient(i) != this)
					inUse = true;
			}
			if (inUse)
			{
				this->sendReply(ERR_ERRONEUSNICKNAME, Error::nicknameinuse(msg.getParam(0)));
				return (-1);
			}
			else
			{
				_hasNickname = true;
				_nickname = msg.getParam(0);
				if (_hasUsername)
				{
					_isRegistered = true;
					this->send_buff(":" + this->_server->getName() + " 001 " + _nickname + " :Connection to server established");
					this->sendReply(RPL_WELCOME, Reply::welcome(_nickname));
				}
			}
		}
	}
	return (0);
}