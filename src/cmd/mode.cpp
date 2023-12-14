#include "Numeric.hpp"
#include "Client.class.hpp"
#include "Channel.class.hpp"
#include "Server.class.hpp"
#include <iostream>
#include <sstream>
#include <stdlib.h>

void	handleChannelMode(char c, int *sign, Server *server, Channel *channel, Client *client, const Message & msg, int *argIndex)
{
	int limit;
	switch (c)
	{
		case '+':
			*sign = 1;
			break ;
		case '-':
			*sign = 0;
			break ;
		case 'i':
			channel->setInviteOnly(*sign == 1);
			if (*sign)
				client->send_buff(":" + server->getName() + " MODE " + channel->getName() + " +i");
			else
				client->send_buff(":" + server->getName() + " MODE " + channel->getName() + " -i");
			break ;
		case 't':
			channel->setMutableTopic(*sign == 1);
			if (*sign)
				client->send_buff(":" + server->getName() + " MODE " + channel->getName() + " +t");
			else
				client->send_buff(":" + server->getName() + " MODE " + channel->getName() + " -t");
			break ;
		case 'k':
			if (*sign)
			{
				if (*argIndex < (int) msg.getParamSize())
				{
					channel->setPassword(msg.getParam(*argIndex++));
					client->send_buff(":" + server->getName() + " MODE " + channel->getName() + " +k " + msg.getParam(*argIndex - 1));
				}
				else
					client->sendReply(ERR_NEEDMOREPARAMS, Error::needmoreparams(msg.getCommand()));
				break ;
			}
			else
			{
				channel->removePassword();
				client->send_buff(":" + server->getName() + " MODE " + channel->getName() + " -k");
			}
			break ;
		case 'o':
			Client	*user;

			if (*argIndex < (int) msg.getParamSize())
				user = channel->getMember(msg.getParam(*argIndex++));
			else
				user = client;
			if (!user)
				client->sendReply(ERR_NOSUCHNICK, Error::nosuchnick(msg.getParam(*argIndex - 1)));
			else if (!*sign)
			{
				if (channel->isOperator(user))
					channel->removeOperator(user);
				client->send_buff(":" + server->getName() + " MODE " + channel->getName() + " -o " + user->getNickname());
			}
			else if (*sign)
			{
				if (!channel->isOperator(user))
					channel->addOperator(user);
				client->send_buff(":" + server->getName() + " MODE " + channel->getName() + " +o " + user->getNickname());
			}
			break ;
		case 'l':
			if (*argIndex < (int) msg.getParamSize())
			{
				limit = atoi(msg.getParam(*argIndex++).c_str());
				channel->setUserLimit(limit);
				client->send_buff(":" + server->getName() + " MODE " + channel->getName() + " +l " + msg.getParam(*argIndex - 1));
			}
			else
			{
				if (*sign)
					client->sendReply(ERR_NEEDMOREPARAMS, Error::needmoreparams(msg.getCommand()));
				else
				{
					channel->setUserLimit(-1);
					client->send_buff(":" + server->getName() + " MODE " + channel->getName() + " -l");
				}
			}
			break ;
		case 'b':
			if (*argIndex < (int) msg.getParamSize())
			{

				user = channel->getMember(msg.getParam(*argIndex++));
				if (!user)
					client->sendReply(ERR_NOSUCHNICK, Error::nosuchnick(msg.getParam(*argIndex - 1)));
				else if (*sign && !channel->isBanned(user))
				{
					if (channel->isOperator(user))
						channel->removeOperator(user);
					if (channel->isMember(user))
					{
						channel->removeMember(user);
						user->send_buff(":" + client->getNickname() + " KICK " + channel->getName() + " " + user->getNickname());
					}
					channel->banMember(user);
					client->send_buff(":" + server->getName() + " MODE " + channel->getName() + " +b " + user->getNickname());
				}
				else if (!*sign)
				{
					channel->unbanMember(user);
					user->send_buff(":" + server->getName() + " MODE " + channel->getName() + " -b " + user->getNickname());
				}
			}
			break ;
		default:
			break;
	}
}

int	Client::mode(const Message& msg)
{
	Channel	*channel;
	int		paramIndex;
	int		sign;
	int		argIndex;

	if (msg.getCommand() != "MODE")
		return (-1);
	else if ((int) msg.getParamSize() < 1)
		this->sendReply(ERR_NEEDMOREPARAMS, Error::needmoreparams(msg.getCommand()));
	else if (msg.getParam(0).at(0) == '#')
	{
		
		channel = _server->getChannel(msg.getParam(0));
		if (!channel)
			sendReply(ERR_NOSUCHCHANNEL, Error::nosuchchannel(msg.getParam(0)));
		else if (!channel->isMember(this))
			sendReply(ERR_NOTONCHANNEL, Error::notonchannel(msg.getParam(0)));
		else if ((int) msg.getParamSize() < 2)
		{
			std::string	modeString = "";
			if (channel->isInviteOnly())
				modeString += "i";
			if (channel->hasMutableTopic())
				modeString += "t";
			if (channel->hasPassword())
				modeString += "k";
			if (channel->getUserLimit() > 0)
				modeString += "l";
			sendReply(RPL_CHANNELMODEIS, Reply::channelmodeis(msg.getParam(0), modeString));
		}
		else
		{
			paramIndex = 1;
			argIndex = -1;
			for (int i = 1; i < (int) msg.getParamSize() && argIndex == -1; i++)
			{
				if (msg.getParam(i).at(0) != '+' && msg.getParam(i).at(0) != '-')
					argIndex = i;
			}
			sign = 1;
			while (paramIndex < (int) msg.getParamSize()
				&& (msg.getParam(paramIndex).at(0) == '+' || msg.getParam(paramIndex).at(0) == '-'))
			{
				for (int i = 0; i < (int) msg.getParam(paramIndex).length(); i++)
					handleChannelMode(msg.getParam(paramIndex).at(i), &sign, _server, channel, this, msg, &argIndex);
				paramIndex++;
			}
		}
	}
	else
	{
		if (msg.getParam(0) == _nickname)
		{
			paramIndex = 1;
			argIndex = -1;
			for (int i = 1; i < (int) msg.getParamSize() && argIndex == -1; i++)
			{
				if (msg.getParam(i).at(0) != '+' && msg.getParam(i).at(0) != '-')
					argIndex = i;
			}
			sign = 1;
			while (paramIndex < (int) msg.getParamSize()
				&& (msg.getParam(paramIndex).at(0) == '+' || msg.getParam(paramIndex).at(0) == '-'))
			{
				for (int i = 0; i < (int) msg.getParam(paramIndex).length(); i++)
				{
					switch (msg.getParam(paramIndex).at(i))
					{
						case '+':
							sign = 1;
							break;
						case '-':
							sign = 0;
							break;
						case 'i':
							_invisible = sign == 1;
							send_buff(":" + _server->getName() + " 221 " + _nickname + (sign ? " +i" : " -i"));
							break;
						default:
							sendReply(ERR_UMODEUNKNOWNFLAG, Error::umodeunknownflag());
							break;

					}
				}
				paramIndex++;
			}
		}
		else
			sendReply(ERR_USERSDONTMATCH, Error::usersdontmatch());
	}
	return (0);
}