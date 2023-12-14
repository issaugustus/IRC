#include "Numeric.hpp"
#include "Client.class.hpp"
#include "Channel.class.hpp"
#include "Server.class.hpp"
#include <iostream>
#include <sstream>

int Client::quit(const Message& msg)
{
    std::string quitMessage;
    std::string channelQuit;
    Channel     *channel;
    Client      *client;

    quitMessage = this->getUserMask() + " QUIT";
    channelQuit = quitMessage;
    if (msg.getParamSize() > 0)
        quitMessage = quitMessage + " :" + msg.getParam(0);
    for (int i = 0; i < (int) this->_channels.size(); i++)
    {
        channel = _server->getChannel(_channels.at(i));
        if (channel)
        {
            if (channel->isMember(this))
            {
                for (int k = 0; k < (int) channel->getNbMember(); k++)
                {
                    client = channel->getMember(k);
                    if (client && client != this)
                        client->send_buff(channelQuit + " " + _channels.at(i) + ((int) msg.getParamSize() > 0 ? msg.getParam(0) : ""));
                }
            }    
        }
    }
    this->send_buff(quitMessage);
    delete this;
    return (0);
}