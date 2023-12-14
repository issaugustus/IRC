#include "Numeric.hpp"
#include "Client.class.hpp"
#include "Channel.class.hpp"
#include "Server.class.hpp"
#include <iostream>
#include <sstream>


int	Client::privmsg(const Message &msg)
{
	std::vector<std::string>	receivers;
	std::string					text;
	Client						*user;
	Channel						*channel;


	if (msg.getCommand() != "PRIVMSG")
		return (1);
	else if (!_isRegistered)
		sendReply(ERR_NOTREGISTERED, Error::notregistered());
	else if ((int) msg.getParamSize() < 1)
		sendReply(ERR_NORECIPIENT, Error::norecipient(msg.getCommand()));
	else if ((int) msg.getParamSize() < 2)
		sendReply(ERR_NOTEXTTOSEND, Error::notexttosend());
	else
	{
		std::istringstream			receiversF(msg.getParam(0));
		std::string					tmp;
		while (getline(receiversF, tmp, ','))
			receivers.push_back(tmp);
		for (int i = 0; i < (int) receivers.size(); i++)
		{
			
			if (receivers.at(i).at(0) != '#')
			{
				user = _server->getClientByNick(receivers.at(i));
				if (!user)
					sendReply(ERR_NOSUCHNICK, Error::nosuchnick(receivers.at(i)));
				else
					user->send_buff(this->getUserMask() + " " + msg.getCommand() + " " + user->getNickname() + " :" + msg.getParam(1));
			}
			else
			{
				channel = _server->getChannel(receivers.at(i));
				if (!channel)
					sendReply(ERR_NOSUCHCHANNEL, Error::nosuchchannel(receivers.at(i)));
				else
				{
					if (!channel->isMember(this))
						sendReply(ERR_CANNOTSENDTOCHAN, Error::cannotsendtochan(channel->getName()));
					else for (int k = 0; k < (int) channel->getNbMember(); k++)
						if (channel->getMember(k) != this)
							channel->getMember(k)->send_buff(this->getUserMask() + " " + msg.getCommand() + " " + channel->getName() + " :" + msg.getParam(1));
				}
			}
		}
		return (0);
	}
	return (-1);
}