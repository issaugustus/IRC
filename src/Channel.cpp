#include "Channel.class.hpp"
#include <algorithm>


Channel::Channel(Server *server, const std::string & name)
{
	_userLimit = -1;
	_server = server;
	_hasPassword = false;
	_inviteOnly = false;
	_mutableTopic = false;
	_topic = "";
	_members.push_back(server->getBoat());
	_operators.push_back(server->getBoat());
	if (name.at(0) != '#' && name.at(0) != '&')
		throw invalidNameException();
	_name = name;
}

Channel::Channel(Server *server, const std::string & name, const std::string & password)
{
	_userLimit = -1;
	_server = server;
	_topic = "";
	if (name.at(0) != '#' && name.at(0) != '&')
		throw invalidNameException();
	_name = name;
	_inviteOnly = false;
	_mutableTopic = false;
	if (!password.size())
		throw emptyPasswordException();
	_password = password;
	_hasPassword = true;
}

Channel::Channel()
{
	_userLimit = -1;
	_topic = "";
	_hasPassword = false;
	_inviteOnly = false;
	_mutableTopic = false;
}

Channel::Channel(const Channel & rhs)
{
	this->_userLimit = rhs._userLimit;
	this->_name = rhs._name;
	this->_topic = rhs._topic;
	this->_hasPassword = rhs._hasPassword;
	if (this->_hasPassword)
		this->_password = rhs._password;
	this->_mutableTopic = rhs._mutableTopic;
	this->_inviteOnly = rhs._mutableTopic;
	this->_members = std::vector<Client *>(rhs._members);
	this->_operators = std::vector<Client *>(rhs._operators);
	this->_banned = std::vector<Client *>(rhs._banned);
	this->_server = rhs._server;
}

Channel & Channel::operator=(const Channel & rhs)
{
	this->_userLimit = rhs._userLimit;
	this->_name = rhs._name;
	this->_topic = rhs._topic;
	this->_hasPassword = rhs._hasPassword;
	if (this->_hasPassword)
		this->_password = rhs._password;
	this->_mutableTopic = rhs._mutableTopic;
	this->_inviteOnly = rhs._mutableTopic;
	this->_members = std::vector<Client *>(rhs._members);
	this->_operators = std::vector<Client *>(rhs._operators);
	this->_banned = std::vector<Client *>(rhs._banned);
	this->_server = rhs._server;
	return (*this);
}

Channel::~Channel() {}

const char	*Channel::emptyPasswordException::what() const throw()
{
	return ("You have to provide a password!");
}

const char	*Channel::noPasswordException::what() const throw()
{
	return ("This channel doesnt have a password!");
}

const char	*Channel::invalidNameException::what() const throw()
{
	return ("Invalid name for a channel");
}

const std::string &	Channel::getTopic() const
{
	return (_topic);
}

const std::string &	Channel::getPassword() const
{
	if (!_hasPassword)
		throw noPasswordException();
	return (this->_password);
}

Client	*Channel::getMember(int index) const
{
	if (index < 0 || index >= (int) _members.size())
		return (NULL);
	return (_members.at(index));
}

Client	*Channel::getOperator(int index) const
{
	if (index < 0 || index >= (int) _operators.size())
		return (NULL);
	return (_operators.at(index));
}

size_t	Channel::getNbMember() const
{
	return (_members.size());
}

size_t	Channel::getNbOperator() const
{
	return (_operators.size());
}

bool	Channel::hasPassword() const
{
	return (_hasPassword);
}

bool	Channel::hasMutableTopic() const
{
	return (_mutableTopic);
}

bool	Channel::isInviteOnly() const
{
	return (_inviteOnly);
}

void	Channel::setTopic(const std::string & topic)
{
	if (topic.size())
		_topic = topic;
	else if (DEBUG)
		std::cerr << "Please provide a non empty topic";
}

void	Channel::setPassword(const std::string & password)
{
	if (!password.size())
		_hasPassword = false;
	else
	{
		_hasPassword = true;
		_password = password;
	}
}

int		Channel::getUserLimit() const
{
	return (_userLimit);
}

void	Channel::setUserLimit(int limit)
{
	this->_userLimit = limit;
}

void	Channel::removePassword()
{
	_hasPassword = false;
}

void	Channel::addOperator(Client *chanop)
{
	if (std::find(_operators.begin(), _operators.end(), chanop) == _operators.end())
		_operators.push_back(chanop);
	else if (DEBUG)
		std::cerr << "Client " << chanop->getUsername() << " is already an operator of channel " << _topic << std::endl;
}

void	Channel::addMember(Client *member)
{
	if (std::find(_members.begin(), _members.end(), member) == _members.end())
	{
		_members.push_back(member);
	}
	else if (DEBUG)
		std::cerr << "Client " << member->getUsername() << " is already a member of channel " << _topic << std::endl;
}

void	Channel::removeMember(int index)
{
	if ((int) _members.size() > index && index >= 0)
		_members.erase(_members.begin() + index);
	else if (DEBUG)
		std::cerr << "Please provide a valid member index\n";
}

void	Channel::removeMember(Client *member)
{
	std::vector<Client *>::iterator	iterator = std::find(_members.begin(), _members.end(), member);
	if (iterator != _members.end())
		_members.erase(iterator);
}

void	Channel::removeOperator(int index)
{
	if ((int) _operators.size() > index && index >= 0)
		_operators.erase(_operators.begin() + index);
	else if (DEBUG)
		std::cerr << "Please provide a valid operator index\n";
}

void	Channel::setMutableTopic(bool value)
{
	_mutableTopic = value;
}

void	Channel::setInviteOnly(bool value)
{
	_inviteOnly = value;
}

bool	Channel::isOperator(Client *client) const
{
	return (std::find(_operators.begin(), _operators.end(), client) != _operators.end());
}

bool	Channel::isOperator(const std::string & nickname) const
{
	for (int i = 0; i < (int) _operators.size(); i++)
	{
		if (_operators.at(i)->getNickname() == nickname)
			return (true);
	}
	return (false);
}

bool	Channel::isMember(Client *client) const
{
	return (std::find(_members.begin(), _members.end(), client) != _members.end());
}

bool	Channel::isMember(const std::string & nickname) const
{
	for (int i = 0; i < (int) _members.size(); i++)
	{
		if (_members.at(i)->getNickname() == nickname)
			return (true);
	}
	return (false);
}

Client	*Channel::getOperator(const std::string & nickname) const
{
	for (int i = 0; i < (int) _operators.size(); i++)
	{
		if (_operators.at(i)->getNickname() == nickname)
			return (_operators.at(i));
	}
	return (NULL);
}

Client	*Channel::getMember(const std::string & nickname) const
{
	for (int i = 0; i < (int) _members.size(); i++)
	{
		if (_members.at(i)->getNickname() == nickname)
			return (_members.at(i));
	}
	return (NULL);
}

void	Channel::invite(Client *client)
{
	if (std::find(_invited.begin(), _invited.end(), client) == _invited.end())
		_invited.push_back(client);
}

bool	Channel::isInvited(Client *client) const
{
	return (std::find(_invited.begin(), _invited.end(), client) != _invited.end());
}

void	Channel::removeInvite(Client *client)
{
	std::vector<Client *>::iterator clientIterator = std::find(_invited.begin(), _invited.end(), client);
	if (clientIterator != _invited.end())
		_invited.erase(clientIterator);
}

bool	Channel::isBanned(Client *client) const
{
	return (std::find(_banned.begin(), _banned.end(), client) != _banned.end());
}

bool	Channel::isBanned(const std::string & nickname) const
{
	Client *member = getMember(nickname);
	return (member && isBanned(member));
}

void	Channel::banMember(Client *client)
{
	if (!isBanned(client))
		_banned.push_back(client);
}

void	Channel::banMember(const std::string & nickname)
{
	Client *member = this->getMember(nickname); // DEVRA ETRE  = _server->getClient()
	if (member)
		banMember(member);
}

void	Channel::unbanMember(Client *client)
{
	std::vector<Client *>::iterator clientIterator = std::find(_banned.begin(), _banned.end(), client);
	if (clientIterator != _banned.end())
		_banned.erase(clientIterator);

}
void	Channel::unbanMember(const std::string & nickname)
{
	for (int i = 0; i < (int) _banned.size(); i++)
	{
		if (_banned.at(i)->getNickname() == nickname)
			_banned.erase(_banned.begin() + i);
	}
}

const std::string &	Channel::getName() const
{
	return (_name);
}

void	Channel::removeOperator(Client *chanop)
{
	for (int i = 0; i < (int) _operators.size(); i++)
	{
		if (_operators.at(i) == chanop)
			_operators.erase(_operators.begin() + i);
	}
}