#include "../../includes/Numeric.hpp"
#include "../../includes/Client.class.hpp"
#include "../../includes/Channel.class.hpp"
#include "../../includes/Server.class.hpp"
#include "../../includes/ErrorCode.hpp"

int Client::invite(const Message& msg)
{
	Channel *channel;
	Client	*client;

	if (msg.getCommand() != "INVITE")
		return (1);
	if ((int) msg.getParamSize() < 2)
    {
		this->sendReply(ERR_NEEDMOREPARAMS, Error::needmoreparams(msg.getCommand()));
        return (-1);
    }
    if (!this->getIsRegistered())
    {
        this->sendReply(ERR_NOTREGISTERED, Error::notregistered());
        return (-1);
    }
	else
	{
		channel = (this->getServer())->getChannel(msg.getParam(1));
		client = (this->getServer())->getClientByNick(msg.getParam(0));
        if (channel == NULL)
        {
            this->sendReply(ERR_NOSUCHCHANNEL, Error::nosuchchannel(msg.getParam(1))); //verifier si channel name est bien en 1 dans params
            return (-1);
        }
		if (!client)
        {
			this->sendReply(ERR_NOSUCHNICK, Error::nosuchnick(msg.getParam(0)));
            return (-1);
        }
		if (!channel->isOperator(this) && channel->isInviteOnly())
        {
			this->sendReply(ERR_CHANOPRISNEED, Error::chanoprisneeded(msg.getParam(1)));
            return (-1);
        }
		if (!channel->isMember(this))
        {
			this->sendReply(ERR_NOTONCHANNEL, Error::notonchannel(msg.getParam(1)));
            return (-1);
        }
		if (channel->isMember(client))
        {
			this->sendReply(ERR_USERONCHANNEL, Error::useronchannel(msg.getParam(0), msg.getParam(1)));
            return (-1);
        }
		this->sendReply(RPL_INVITING, Reply::inviting(msg.getParam(1), msg.getParam(0)));
		client->send_buff(this->getUserMask() + " INVITE " + msg.getParam(0) + " " + msg.getParam(1));
		channel->invite(client);
        if (client->getIsAway())
        {
            this->sendReply(RPL_AWAY, Reply::away(client->getNickname(), client->getAwayMessage()));
        }
        //tricky condition, je sais pas si elle est a sa place ici ou faut la monter
        if (channel->isBanned(client))
            channel->unbanMember(client);
	}
	return (0);
}
