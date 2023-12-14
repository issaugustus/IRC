#include "../../includes/Numeric.hpp"
#include "../../includes/Client.class.hpp"
#include "../../includes/Channel.class.hpp"
#include "../../includes/Server.class.hpp"
#include "../../includes/ErrorCode.hpp"
//TODO check reply after topic is set
int Client::topic(const Message& msg)
{
    if (!this->getIsRegistered())
    {
        this->sendReply(ERR_NOTREGISTERED, Error::notregistered());
        return (-1);
    }
    if (msg.getParamSize() < 1)
    {
        this->sendReply(ERR_NEEDMOREPARAMS, Error::needmoreparams(msg.getCommand()));
        return (-1);
    }
    //TO DO Check si channel exist et si this est operateur channel // done
    Channel *c = (this->getServer())->getChannel(msg.getParam(0));
    if (!c)
    {
        this->sendReply(ERR_NOSUCHCHANNEL, Error::nosuchchannel(msg.getParam(0)));
        return (-1);
    }
    if (!c->hasMutableTopic() && !c->isOperator(this))
    {
        this->sendReply(ERR_CHANOPRISNEED, Error::chanoprisneeded(c->getName()));
        return (-1);
    }
    if (!c->isMember(this))
    {
        this->sendReply(ERR_NOTONCHANNEL, Error::notonchannel(c->getName()));
        return (-1);
    }
    if (msg.getParamSize() >= 2)
        c->setTopic(msg.getParam(1));
    if (msg.getParamSize() == 1)
    {
        if (!c->getTopic().empty())
            this->sendReply(RPL_TOPIC, Reply::topic(c->getName(), c->getTopic()));
        else
            this->sendReply(RPL_NOTOPIC, Reply::notopic(c->getName()));
    }
    return (0);
    
}