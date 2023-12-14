#include "Client.class.hpp"
#include "Message.class.hpp"
#include "ErrorCode.hpp"
#include "Server.class.hpp"
#include <arpa/inet.h>
#include <string>
#include <cstring>
#include <map>

//add channels vector initialization
Client::Client(Server *server, const char * ipAddress)
{
	_server = server;
	_isOperator = false;
	_isRegistered = false;
	_isAway = false;
	_servNotice = false;
	_invisible = false;
	_restricted = false;
	_hasPassword = false;
	_hasPassword = false;
	_hasNickname = false;
	_hasUsername = false;
	_isUp = true;
	_lastActivity = std::time(0);

	_servername = "ft_irc";
	_password = "";
	_hostname = "default hostname";
	_awayMessage = "I'm away";
	_ipAddress = inet_addr(ipAddress);
	_buffReceive = "";
}

Client::Client()
{
	_lastActivity = std::time(0);
	_server = NULL;
	_isOperator = false;
	_isRegistered = false;
	_isAway = false;
	_servNotice = false;
	_invisible = false;
	_restricted = false;
	_hasPassword = false;
	_hasNickname = false;
	_hasUsername = false;

	_servername = "ft_irc";
	_nickname = "Default username";
	_name = "Default username";
	_username = "Default username";
	_hostname = "default hostname";
	_awayMessage = "I'm away";
	_ipAddress = inet_addr("127.0.0.1");
	_buffReceive = "";
}

Client & Client::operator=(const Client & rhs)
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

Client::Client(const Client & rhs)
{
	if (this != &rhs)
		*this = rhs;
}


Client::~Client()
{
	Channel	*channel;

	if (this != _server->getBoat())
	{
		for (int i = 0; i < (int) this->_channels.size(); i++)
		{
			channel = this->_server->getChannel(this->_channels.at(i));
			if (channel)
			{
				if (channel->isMember(this))
					channel->removeMember(this);
				if (channel->isOperator(this))
					channel->removeOperator(this);
			}
		}
		_server->disconnectClient(this);
	}
	if (DEBUG)
		std::cout << "Disconnected client " << this->_nickname << " and deleted its data\n";
}


//====================Getters===================

Server*             Client::getServer() const
{
    return (this->_server);
}
const std::string &	Client::getUsername() const
{
	return (this->_username);
}

const std::string &	Client::getNickname() const
{
	return (this->_nickname);
}

const std::string &	Client::getName() const
{
	return (this->_name);
}

const std::string &	Client::getHostname() const
{
	return (this->_hostname);
}

const std::string &	Client::getAwayMessage() const
{
	return (this->_awayMessage);
}

const std::string Client::getUserMask() const
{
	return (":" + this->_nickname + "!" + this->_username + "@" + this->_hostname);
}

const in_addr_t & Client::getIpAddress() const
{
	return (this->_ipAddress);
}

const int & 	Client::getSocket() const
{
	return (this->_socket);
}

bool			Client::getIsOperator() const
{
	return (this->_isOperator);
}

bool			Client::getIsRegistered() const
{
	return (this->_isRegistered);
}

bool			Client::getIsAway() const
{
	return (this->_isAway);
}

int				Client::getDownTime() const
{
    std::time_t currentTime = std::time(0);
	return (currentTime - _lastActivity);
}

std::string&	Client::getBuffReceive()
{
	return (this->_buffReceive);
}

void			Client::setBuffReceive(std::string buff)
{
	this->_buffReceive = buff;
}

void			Client::appendBuffReceive(std::string buff)
{
	this->_buffReceive += buff;
}
//====================Functions=================

//A TESTER J AI TROUVE QUE LE CAST DANS STRINGSTREAM JSP SI CA MARCHE BIEN C++98
void	Client::sendReply(NumericCode num, const std::string & message)
{
    std::stringstream ss;
    ss << static_cast<int>(num);
	send_buff(":" + _servername + " " + ss.str() + " " + _nickname + " " + message); // Ajouter le code d'erreur entre servername et nickname
}

void		Client::send_buff(const std::string& message)
{
	// std::cout << "Sending a message to " << this->_socket << std::endl;
	std::string msg;
	msg = message + "\r\n";
	const char* buffer = msg.c_str();
	if (DEBUG)
		std::cout << "Sending : " << message << " to user : " << this->_nickname << std::endl;
	size_t		totalBytesSent = 0;
	size_t		length = msg.length();

	while(totalBytesSent < length)
	{
		size_t sent = write(this->_socket, buffer + totalBytesSent, length - totalBytesSent);

		if ((int) sent == -1)
		{
			if (DEBUG)
				std::cerr << "Error while sending message, check your connexion" << std::endl;
			break ;
		}
		totalBytesSent += sent;
	}
}

//rewrite with the channel class in argument
void		Client::join_channel(std::string& channel_name)
{
	this->_channels.push_back(channel_name);
	if (DEBUG)
		std::cout << "Joining the channel " << channel_name  << std::endl;
}

void		Client::leave_channel(std::string& channel_name)
{
	for (int i = 0; i < (int) _channels.size(); i++)
	{
		if (_channels.at(i) == channel_name)
			_channels.erase(_channels.begin() + i);
	}
	if (DEBUG)
		std::cout << "Quitting the channel " << channel_name << std::endl;
}

void		Client::change_nickname(std::string& nickname)
{
	if (this->_nickname.compare(nickname))
		this->_nickname = nickname;
	else if (DEBUG)
	{
		std::cerr << "Same nickname, no need to change it" << std::endl;
	}
}

void		Client::set_away(std::string& away_message)
{
	if (!this->_isAway)
	{
		this->_isAway = true;
		this->_awayMessage = away_message;
		std::cout << "Send the away message to what ? " << std::endl;
	}
	else if (DEBUG)
	{
		std::cerr << "User " << this->_nickname << " is already away" << std::endl;
	}
}

std::string	toUpper(const std::string &str)
{
	std::string	upper;

	for (int i = 0; i < (int) str.size(); i++)
	{
		upper += toupper(str[i]);
	}
	return (upper);
}

int			Client::executeCommand(const Message &msg)
{
	this->resetDownTime();
	if (DEBUG)
	{

		std::cout << "Command : " << msg.getCommand() << "\nArguments : ";
		for (int i = 0; i < (int) msg.getParamSize() - 1; i++)
		{
			std::cout << msg.getParam(i) << ' ';
		}
		if (msg.getParamSize() > 0)
			std::cout << msg.getParam(msg.getParamSize() - 1) << std::endl;
		else
			std::cout << std::endl;
	}
	if (!_isRegistered && Message::unregisteredCommands.find(msg.getCommand()) == Message::unregisteredCommands.end())
	{
		sendReply(ERR_NOTREGISTERED, Error::notregistered());
		return (E_UNKNOWN_COMMAND);
	}
	if (Message::commandMap.find(msg.getCommand()) != Message::commandMap.end())
	{
		return ((this->*Message::commandMap.find(toUpper(msg.getCommand()))->second)(msg));
	}
	return (E_UNKNOWN_COMMAND);
}

//add an operator special password
void		Client::set_operator_mode(std::string& password)
{
	if (this->_server->getOperatorPassword().compare(password) != 0)
	{
		if (DEBUG)
			std::cerr << "Sorry, wrong password..." << std::endl;
	}	
	else
	{
		this->_isOperator = true;
	}
}

void Client::setSocket(int socket)
{
	this->_socket = socket;
}

void		Client::resetDownTime()
{
	this->_isAway = false;
	this->_lastActivity = std::time(0);
}
