#include "Numeric.hpp"
#include "Client.class.hpp"
#include "Channel.class.hpp"
#include "Server.class.hpp"
#include "Message.class.hpp"
#include <iostream>
#include <sstream>

int	Client::join(const Message & msg)
{
	Channel 					*channel;
	std::vector<std::string>	channels;
	std::vector<std::string>	passwords;
	std::string					tmp;
	bool						hasPassword = false;
	std::string					password;

	if (msg.getCommand() != "JOIN")
	{
		return (1);
	}
	else if ((int) msg.getParamSize() < 1 || msg.getParam(0) == "")
	{
		this->sendReply(ERR_NEEDMOREPARAMS, Error::needmoreparams(msg.getCommand()));
	}
	else
	{
		std::istringstream	channelsF(msg.getParam(0));
		if ((int) msg.getParamSize() >= 2)
		{
			std::istringstream	passwordF(msg.getParam(1));
			while (getline(passwordF, tmp, ','))
				passwords.push_back(tmp);
		}
		while (getline(channelsF, tmp, ','))
			channels.push_back(tmp);
		for (int i = 0; i < (int) channels.size(); i++)
		{
			channel = _server->getChannel(channels.at(i));
			if (i < (int) passwords.size())
			{
				hasPassword = true;
				password = passwords.at(i);
			}
			if (!channel)
			{
				try
				{
					if (hasPassword)
						channel = new Channel(_server, channels.at(i), passwords.at(i));
					else
						channel = new Channel(_server, channels.at(i));
					_server->addChannel(channel);
					channel->addMember(this);
					channel->addOperator(this);
					this->_channels.push_back(channels.at(i));
					this->send_buff(this->getUserMask() + " JOIN " + channels.at(i));
				}
				catch(const Channel::invalidNameException& e)
				{
					this->sendReply(ERR_NOSUCHCHANNEL, Error::nosuchchannel(channels.at(i)));
				}
			}
			else if (channel->isInviteOnly() && !channel->isInvited(this))
				this->sendReply(ERR_INVITEONLYCHAN, Error::inviteonlychan(channels.at(i)));
			else if (channel->isBanned(this))
				this->sendReply(ERR_BANNEDFROMCHAN, Error::bannedfromchan(channels.at(i)));
			else if ((channel->hasPassword() && !hasPassword) || (channel->hasPassword() && password != channel->getPassword()))
				this->sendReply(ERR_BADCHANNELKEY, Error::badchannelkey(channels.at(i)));
			else if (channel->getUserLimit() <= (int) channel->getNbMember() && channel->getUserLimit() > 0)
				this->sendReply(ERR_CHANNELISFULL, Error::channelisfull(channels.at(i)));
			else
			{
				sendReply(RPL_TOPIC, Reply::topic(channels.at(i), channel->getTopic()));
				std::string	members("");
				for (int j = 0; j < (int) channel->getNbMember() - 1; j++)
					members = members + channel->getMember(j)->getNickname() + ' ';
				if (channel->getNbMember() > 0)
					members = members + (channel->getMember((int) channel->getNbMember() - 1))->getNickname();
				sendReply(RPL_NAMREPLY, Reply::namreply(members));
				this->send_buff(this->getUserMask() + " JOIN " + channels.at(i));
				if (!channel->isMember(this))
				{
					_channels.push_back(channels.at(i));
					for (int j = 0; j < (int) channel->getNbMember(); j++)
						channel->getMember(j)->send_buff(this->getUserMask() + " JOIN " + channels.at(i));
					channel->addMember(this);
				}
			}
		}
	}
	return (0);
}