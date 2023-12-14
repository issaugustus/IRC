#include "../../includes/Numeric.hpp"
#include "../../includes/Client.class.hpp"
#include "../../includes/Channel.class.hpp"
#include "../../includes/Server.class.hpp"
#include "../../includes/ErrorCode.hpp"

//EDIT: je suis parti en freestyle, c'est 1 KICK par kick, pour kick plusieurs users
//faut spam de commandes KICK
int	Client::kick(const Message & msg)
{
	if (msg.getCommand() != "KICK")
		return (1);
    if (!this->getIsRegistered())
    {
        this->sendReply(ERR_NOTREGISTERED, Error::notregistered());
        return (-1);
    }
	else
	{
        //verif si on traite pas quand meme tout les kick jusqu'au dernier ou il manque 1 param
		if ((int) msg.getParamSize() < 2)//why le  ?? params parsing = 1 parametre par case? ca risque de casser la condition car il peut y avoir 3 arguments(3eme c raison du kick)
        {
			sendReply(ERR_NEEDMOREPARAMS, Error::needmoreparams(msg.getCommand()));
            return (-1);
        }
        Channel *channel = (this->getServer())->getChannel(msg.getParam(0));
        if (!channel)
        {
            this->sendReply(ERR_NOSUCHCHANNEL, Error::nosuchchannel(msg.getParam(0)));
            return (-1);
        }
        if (!channel->isOperator(this))
        {
            this->sendReply(ERR_CHANOPRISNEED, Error::chanoprisneeded(channel->getName()));
            return (-1);
        }
        if (channel->getNbOperator() == 0)
        {
            this->sendReply(ERR_NOCHANMODES, Error::nochanmodes(msg.getParam(0)));
            return (-1);
        }
        Client  *client = (this->getServer())->getClientByNick(msg.getParam(0));
        if (!channel->isMember(this))
        {
            this->sendReply(ERR_NOTONCHANNEL, Error::notonchannel(channel->getName()));
            return (-1);
        }
        if (!client || !channel->isMember(client))
        {
            this->sendReply(ERR_NOTONCHANNEL, Error::notonchannel(channel->getName()));
            return (-1);
        }
        //sinon on KICK SA MERE
        std::string kickedReply = ":" + (this->getServer())->getName() + " " + client->getNickname();
        kickedReply += " :You've been kicked for : ";
        if (msg.getParamSize() > 2)
            kickedReply+= msg.getParam(2);
        else
            kickedReply += "No reason";
        client->send_buff(kickedReply);
        channel->banMember(client);
	}
    return (0);
}