#include "Message.class.hpp"
#include "Client.class.hpp"
#include <string>
#include <iostream>


//////////////////////////////////////
/************************************/
/* 			Message Class 			*/
/************************************/
//////////////////////////////////////

// Liste de toutes les commandes a implementer obligatoirement d'apres la RFC 1459

std::map<std::string, Message::functionPointer>	Message::commandMap;
std::map<std::string, Message::functionPointer>	Message::unregisteredCommands;

void	initializeCommandMap()
{
	Message::commandMap.insert(std::make_pair(std::string("PASS"), &Client::pass));
    Message::commandMap.insert(std::make_pair(std::string("NICK"), &Client::nick));
    Message::commandMap.insert(std::make_pair(std::string("USER"), &Client::user));
    // Message::commandMap.insert(std::make_pair(std::string("SERVER"), &Client::server));
    Message::commandMap.insert(std::make_pair(std::string("OPER"), &Client::oper));
    Message::commandMap.insert(std::make_pair(std::string("QUIT"), &Client::quit));
    // Message::commandMap.insert(std::make_pair(std::string("SQUIT"), &Client::squit));
    Message::commandMap.insert(std::make_pair(std::string("JOIN"), &Client::join));
    Message::commandMap.insert(std::make_pair(std::string("PART"), &Client::part));
    Message::commandMap.insert(std::make_pair(std::string("MODE"), &Client::mode));
    Message::commandMap.insert(std::make_pair(std::string("TOPIC"), &Client::topic));
    // Message::commandMap.insert(std::make_pair(std::string("NAMES"), &Client::names));
    // Message::commandMap.insert(std::make_pair(std::string("LIST"), &Client::list));
    Message::commandMap.insert(std::make_pair(std::string("INVITE"), &Client::invite));
    Message::commandMap.insert(std::make_pair(std::string("KICK"), &Client::kick));
    // Message::commandMap.insert(std::make_pair(std::string("VERSION"), &Client::version));
    // Message::commandMap.insert(std::make_pair(std::string("STATS"), &Client::stats));
    // Message::commandMap.insert(std::make_pair(std::string("LINKS"), &Client::links));
    // Message::commandMap.insert(std::make_pair(std::string("TIME"), &Client::time));
    // Message::commandMap.insert(std::make_pair(std::string("CONNECT"), &Client::connect));
    // Message::commandMap.insert(std::make_pair(std::string("TRACE"), &Client::trace));
    // Message::commandMap.insert(std::make_pair(std::string("ADMIN"), &Client::admin));
    Message::commandMap.insert(std::make_pair(std::string("INFO"), &Client::info));
    Message::commandMap.insert(std::make_pair(std::string("PRIVMSG"), &Client::privmsg));
    Message::commandMap.insert(std::make_pair(std::string("NOTICE"), &Client::notice));
    // Message::commandMap.insert(std::make_pair(std::string("WHO"), &Client::who));
    Message::commandMap.insert(std::make_pair(std::string("WHOIS"), &Client::whois));
    // Message::commandMap.insert(std::make_pair(std::string("WHOWAS"), &Client::whowas));
    Message::commandMap.insert(std::make_pair(std::string("KILL"), &Client::kill));
	Message::commandMap.insert(std::make_pair(std::string("PING"), &Client::ping));
    Message::commandMap.insert(std::make_pair(std::string("PONG"), &Client::pong));
	Message::unregisteredCommands.insert(std::make_pair(std::string("PASS"), &Client::pass));
    Message::unregisteredCommands.insert(std::make_pair(std::string("NICK"), &Client::nick));
    Message::unregisteredCommands.insert(std::make_pair(std::string("USER"), &Client::user));
}
const char	*Message::InvalidMessageException::what() const throw()
{
	return ("InvalidMessage");
}

Message::InvalidCommandException::InvalidCommandException(const char *msg)
{
	_msg = std::string(msg);
}

Message::InvalidCommandException::InvalidCommandException(std::string & msg)
{
	_msg = std::string(msg);
}

Message::InvalidCommandException::InvalidCommandException(char *msg)
{
	_msg = std::string(msg);
}

const char	*Message::InvalidCommandException::what() const throw()
{
	return (_msg.c_str());
}

Message::Message(Client *sender, const std::string & content)
{
	if (sender && content.size() > 0)
	{
		_content = content;
		_sender = sender;
		_prefix = Message::Prefix(content);
		_command = Message::Command(content);
		_params = Message::Params(content);
	}
	else
	{
		if (DEBUG)
			std::cerr << "message : " << content << std::endl;
		throw InvalidMessageException();
	}
}

Message::Message(Client *sender, const char *content)
{
	_content = std::string(content);
	_sender = sender;
	_prefix = Message::Prefix(_content);
	_command = Message::Command(_content);
	_params = Message::Params(_content);
}

Message::Message(const Message & rhs)
{
	_content = rhs._content;
	_sender = rhs._sender;
	_prefix = rhs._prefix;
	_command = rhs._command;
	_params = rhs._params;
}

Message & Message::operator=(const Message & rhs)
{
	_content = rhs._content;
	_sender = rhs._sender;
	_prefix = rhs._prefix;
	_command = rhs._command;
	_params = rhs._params;
	return (*this);
}

Message::~Message() {}
Message::Message() {}

size_t	Message::getParamSize() const
{
	return (_params.getSize());
}

const std::string & Message::getParam(int index) const
{
	return (_params.getParam(index));
}

const std::string &	Message::getParameters() const
{
	return (_params.getContent());
}

const std::string &	Message::getPrefix() const
{
	return (_prefix.getContent());
}

const std::string &	Message::getCommand() const
{
	return (_command.getContent());
}

const Client	*Message::getSender() const
{
	return (_sender);
}

/*
bool	Message::isValid() const
{
	return (_prefix.isValid() && _command.isValid() && _params.isValid() && _command.isValidParams(_params));
}*/

//////////////////////////////////////
/************************************/
/* AMessageComponent Abstract Class */
/************************************/
//////////////////////////////////////

Message::AMessageComponent::AMessageComponent()
{
	_isEmpty = true;
	_content = std::string();
}

bool Message::AMessageComponent::isEmpty() const
{
	return (_isEmpty);
}

const std::string & Message::AMessageComponent::getContent() const
{
	return (_content);
}

Message::AMessageComponent::~AMessageComponent() {}

//////////////////////////////////////
/************************************/
/* 			Prefix Class 			*/
/************************************/
//////////////////////////////////////

Message::Prefix::Prefix(const std::string & messageContent) : Message::AMessageComponent()
{
	size_t	prefixEnd;
	size_t	userBegin;
	size_t	hostBegin;
	_hasUser = true;
	_hasHost = true;

	if ((int) messageContent.size() > 0 && messageContent[0] == ':')
	{
		prefixEnd = messageContent.find(' ');
		this->_content = messageContent.substr(1, prefixEnd - 1);
		this->_isEmpty = prefixEnd == 1;
		if (!_isEmpty)
		{
			userBegin = _content.find('!');
			hostBegin = _content.find('@');
			if (userBegin == std::string::npos)
				_hasUser = false;
			if (hostBegin == std::string::npos)
				_hasHost = false;
			if (_hasUser && _hasHost)
			{
				if (userBegin < hostBegin)
				{
					_host = _content.substr(hostBegin + 1);
					_user = _content.substr(userBegin + 1, hostBegin - userBegin);
				}
				else
				{
					_host = _content.substr(hostBegin + 1, userBegin - hostBegin);
					_user = _content.substr(userBegin + 1);
				}
			}
			else if (_hasUser)
				_user = _content.substr(userBegin + 1);
			else if (_hasHost)
				_host = _content.substr(hostBegin + 1);
			_pfx = _content.substr(0, std::min(userBegin, hostBegin));
			_isNick = _pfx.find('.') == std::string::npos;
		}
	}
	else
		this->_isEmpty = true;
}

Message::Prefix::Prefix(const Message::Prefix & rhs) : Message::AMessageComponent()
{
	this->_content = rhs._content;
	this->_isEmpty = rhs._isEmpty;
}

Message::Prefix & Message::Prefix::operator=(const Message::Prefix & rhs)
{
	this->_content = rhs._content;
	this->_isEmpty = rhs._isEmpty;
	return (*this);
}

Message::Prefix::~Prefix() {}


//////////////////////////////////////
/************************************/
/* 			Command Class 			*/
/************************************/
//////////////////////////////////////

Message::Command::Command(const std::string & messageContent) : Message::AMessageComponent()
{
	size_t	commandBegin;
	size_t	commandEnd;

	if (messageContent.size() <= 0)
	{
		this->_content = std::string();
		this->_isEmpty = true;
	}
	else 
	{
		if (messageContent[0] == ':')
			commandBegin = messageContent.find(' ') + 1;
		else
			commandBegin = 0;
		commandEnd = messageContent.find(' ', commandBegin);
		if (commandEnd == std::string::npos)
			commandEnd = messageContent.size() - 1;
		_content = messageContent.substr(commandBegin, commandEnd - commandBegin);
		_isEmpty = _content.size() == 0;
	}
	if (_isEmpty || !isValid())
	{
		if (DEBUG)
		{
			if (_isEmpty)
				std::cerr << "message is empty\n";
			else
				std::cerr << "message is not valid : " << messageContent << std::endl;
		}
		throw InvalidMessageException();
	}
}

Message::Command::Command(const Message::Command & rhs) : Message::AMessageComponent()
{
	this->_content = rhs._content;
	this->_isEmpty = rhs._isEmpty;
}

Message::Command & Message::Command::operator=(const Message::Command & rhs)
{
	this->_content = rhs._content;
	this->_isEmpty = rhs._isEmpty;
	return (*this);
}

Message::Command::~Command() {}

std::string	toUpper(const std::string &str);

bool	Message::Command::isValid() const
{
	return (commandMap.find(toUpper(_content)) != commandMap.end());
}

//////////////////////////////////////
/************************************/
/* 			Params Class 			*/
/************************************/
//////////////////////////////////////

Message::Params::Params(const std::string & messageContent) : Message::AMessageComponent()
{
	size_t	commandBegin;
	size_t	commandEnd;
	int		i = 0;
	int		j;

	if (messageContent.size() == 0)
	{
		this->_content = std::string();
		this->_isEmpty = true;
	}
	else
	{
		if (messageContent[0] == ':')
			commandBegin = messageContent.find(' ') + 1;
		else
			commandBegin = 0;
		commandEnd = messageContent.find(' ', commandBegin);
		if (commandEnd != std::string::npos)
		{
			_content = messageContent.substr(commandEnd + 1);
			_isEmpty = _content.size() == 0;
		}
		else
		{
			_content = std::string();
			_isEmpty = true;
		}
		if (isValid())
		{
			while (i + 2 < (int) _content.size())
			{
				if (_content.at(i) == ':')
				{
					j = i + 1;
					while (i + 2 < (int) _content.size() && _content.at(i) != '\r' && _content.at(i) != '\n')
						i++;
					_parameters.push_back(_content.substr(j, i - j));
				}
				else
				{
					j = i;
					while (i + 2 < (int) _content.size() && _content.at(i) != ' ' && _content.at(i) != '\r' && _content.at(i) != '\n')
						i++;
					_parameters.push_back(_content.substr(j, i - j));
				}
				i++;
			}
		}
		else if (DEBUG)
		{
			std::cout << "invalid parameters\n";
		}
	}
}

Message::Params::Params(const Message::Params & rhs) : Message::AMessageComponent()
{
	*this = rhs;
}

Message::Params & Message::Params::operator=(const Message::Params & rhs)
{
	if (this != &rhs)
	{
		this->_content = rhs._content;
		this->_isEmpty = rhs._isEmpty;
		this->_parameters = rhs._parameters;
	}
	return (*this);
}

Message::Params::~Params() {}

bool	Message::Params::isValid() const
{
	int	i = 0;
	if (isEmpty() || (int) _content.size() < 2)
	{
		if (DEBUG)
			std::cerr << "EMPTY message params\n";
		return (false);
	}
	while (i + 2 < (int) _content.size())
	{
		if (_content.at(i) == ':')
		{
			while (i + 2 < (int) _content.size() && _content.at(i) != '\r' && _content.at(i) != '\n')
				i++;
			if (i + 2 < (int) _content.size())
			{
				if (DEBUG)
				{
					for (int j = i; j < (int) _content.size(); j++)
						std::cout << static_cast<int>(_content.at(j)) << ", ";
					std::cout << "eol or cr before end of message\n";
				}
				return (false);
			}
		}
		else
		{
			while (i + 2 < (int) _content.size() && _content.at(i) != ' ' && _content.at(i) != '\r' && _content.at(i) != '\n')
				i++;
			if (i + 2 < (int) _content.size() && _content.at(i) != ' ')
			{
				if (DEBUG)
				{
					for (int j = i; j < (int) _content.size(); j++)
						std::cout << static_cast<int>(_content.at(j)) << ", ";
					std::cout << "eol or cr before end of message\n";
				}
				return (false);
			}
		}
		if (i + 2 < (int) _content.size())
			i++;
	}
	if (_content.at(i) == '\r' && _content.at(i + 1) == '\n')
		return (true);
	if (DEBUG)
		std::cout << "not ending with crlf : instead :" << static_cast<int> (_content.at(i)) << ", " << static_cast<int>(_content.at(i +1)) << std::endl;
	return (false);
}

const std::string &	Message::Params::getParam(int index) const
{
	return (_parameters.at(index));
}

size_t	Message::Params::getSize() const
{
	return (_parameters.size());
}

