#include "Bot.class.hpp"
#include "Channel.class.hpp"
#include "Server.class.hpp"
#include "Numeric.hpp"
#include <cstring>

Bot::Bot()
{
	_username = "IRCBOAT";
	_nickname = "IRCBOAT";
	_name = "IRCBOAT";
}

Bot::Bot(Server *server)
{
	this->_username = "IRCBOAT";
	this->_nickname = "IRCBOAT";
	this->_server = server;
	this->_hostname = this->_server->getName();
	this->_name = "IRCBOAT";
}

Bot::Bot(const Bot & rhs) : Client(rhs) {}

Bot & Bot::operator=(const Bot & rhs)
{
	if (this != &rhs)
	{
		this->_isUp = rhs._isUp;
		this->_server = rhs._server;
		this->_nickname = rhs._nickname;
		this->_name = rhs._name;
		this->_username = rhs._username;
		this->_hostname = rhs._hostname;

		this->_servername = rhs._servername;
		this->_isOperator = rhs._isOperator;
		this->_isRegistered = rhs._isRegistered;
		this->_isAway = rhs._isAway;

		this->_ipAddress = rhs._ipAddress;
		this->_awayMessage = rhs._awayMessage;
		this->_channels = rhs._channels;
		this->_buffReceive = rhs._buffReceive;
	}
	return (*this);
}

Bot::~Bot()
{
	_server->disconnectBot(this);
	if (DEBUG)
		std::cout << "Disconnected bot " << this->_nickname << std::endl;
}

void		Bot::sendReply(NumericCode num, const std::string & reply)
{
	(void) num;
	this->send_buff(reply);
}
		
void		Bot::send_buff(const std::string& message)
{
	Message msg(this, message + "\r\n");
	Channel	*channel;
	Client	*client;
	std::string	response = this->getUserMask() + " NOTICE ";

	if (msg.getCommand() == "PRIVMSG" && msg.getParamSize() >= 2)
	{
		channel = _server->getChannel(msg.getParam(0));
		if (channel && msg.getParam(1).find("b") != std::string::npos)
		{
			response += msg.getParam(0) + " :BOAT BOAT BOAT BOAT BOAT";
			for (int i = 0; i < (int) channel->getNbMember(); i++)
			{
				client = channel->getMember(i);
				if (client)
					client->send_buff(response);
			}
		}
	}
}