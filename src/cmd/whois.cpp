#include "Numeric.hpp"
#include "Client.class.hpp"
#include "Channel.class.hpp"
#include "Server.class.hpp"
#include <iostream>
#include <sstream>

int	Client::whois(const Message & msg)
{
	Client 						*user;
	std::vector<std::string>	nickmasks;
	std::string					tmp;
	std::istringstream			nickmasksF;

	if (msg.getCommand() != "WHOIS")
		return (1);
	else if ((int) msg.getParamSize() < 1)
	{
		this->sendReply(ERR_NEEDMOREPARAMS, Error::needmoreparams(msg.getCommand()));
		return (-1);
	}
	else
	{
		if (msg.getParamSize() == 1)
			nickmasksF.str(msg.getParam(0));
		else if (msg.getParam(0) == this->_server->getName() || msg.getParam(0) == this->_servername)
			nickmasksF.str(msg.getParam(1));
		else
		{
			this->sendReply(ERR_NOSUCHSERVER, Error::nosuchserver(msg.getParam(0)));
			return (-1);
		}
		while (getline(nickmasksF, tmp, ','))
			nickmasks.push_back(tmp);
		for (int i = 0; i < (int) nickmasks.size(); i++)
		{
			user = _server->getClientByNick(nickmasks.at(i));
			if (!user)
			{
				sendReply(ERR_NOSUCHNICK, Error::nosuchnick(nickmasks.at(i)));
			}
			else if (user->_invisible)
			{
				send_buff(":" + _server->getName() +  " 311 " + _nickname + " " +  user->_nickname + ": " + user->_name + " (user is invisible)");
			}
			else
			{
				sendReply(RPL_WHOISUSER, Reply::whoisuser(user->getNickname(), user->getUsername(), user->getHostname(), user->getName()));
				if (user->_isAway)
					sendReply(RPL_AWAY, Reply::away(user->getNickname(), user->_awayReason));
				if (!user->_isUp)
					sendReply(RPL_WHOISIDLE, Reply::whoisidle(user->getNickname(), user->getDownTime()));
				sendReply(RPL_WHOISSERVER, Reply::whoisserver(user->getNickname(), _server->getName(), "test"));
				tmp = "";
				for (int i = 0; i < (int) user->_channels.size() - 1; i++)
				{
					if (_server->getChannel(user->_channels.at(i))->isOperator(user))
						tmp = tmp + "@" + user->_channels.at(i) + " ";
					else
						tmp = tmp + user->_channels.at(i) + " ";
				}
				if ((int) user->_channels.size() > 0)
				{
					if (_server->getChannel(user->_channels.back())->isOperator(user))
						tmp = tmp + "@" + user->_channels.back();
					else
						tmp = tmp + user->_channels.back();
					sendReply(RPL_WHOISCHANNELS, Reply::whoischannels(user->getNickname(), tmp));
				}
				if (user->_isOperator)
					sendReply(RPL_WHOISOPERATOR, Reply::whoisoperator(user->getNickname()));
				sendReply(RPL_ENDOFWHOIS, Reply::endofwhois(user->getNickname()));
			}
		}
	}
	return (0);
}