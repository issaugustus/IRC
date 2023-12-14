#include "Numeric.hpp"
#include "Client.class.hpp"
#include "Channel.class.hpp"
#include "Server.class.hpp"
#include <iostream>
#include <sstream>

int		Client::part(const Message & msg)
{
	Channel 					*channel;
	std::vector<std::string>	channels;
	std::string					tmp;

	if (msg.getCommand() != "PART")
		return (1);
	else if ((int) msg.getParamSize() < 1)
		this->sendReply(ERR_NEEDMOREPARAMS, Error::needmoreparams(msg.getCommand()));
	else
	{
		std::istringstream	channelsF(msg.getParam(0));
		while (getline(channelsF, tmp, ','))
			channels.push_back(tmp);
		for (int i = 0; i < (int) channels.size(); i++)
		{
			channel = _server->getChannel(channels.at(i));
			if (!channel)
			{
				sendReply(ERR_NOSUCHCHANNEL, Error::nosuchchannel(channels.at(i)));
			}
			else if (!channel->isMember(this))
			{
				sendReply(ERR_NOTONCHANNEL, Error::notonchannel(channels.at(i)));
			}
			else
			{
				if (msg.getParamSize() >= 2)
					send_buff(":" + this->_server->getName() + " PART " + channels.at(i) + " " + _nickname + " :" + msg.getParam(1));
				else
					send_buff(":" + this->_server->getName() + " PART " + channels.at(i) + " " + _nickname);
				if (msg.getParamSize() >= 2)
					tmp = this->getUserMask() + " PART " + channels.at(i) + " :" + msg.getParam(1);
				else
					tmp = this->getUserMask() + " PART " + channels.at(i);
				for (int i = 0; i < (int) channel->getNbMember(); i++)
				{
					channel->getMember(i)->send_buff(tmp);
				}
				channel->removeMember(this);
				this->leave_channel(channels.at(i));
			}
		}
	}
	return (0);
}