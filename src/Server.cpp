#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "Server.class.hpp"
#include "Channel.class.hpp"
#include "Client.class.hpp"
#include "Message.class.hpp"
#include "Bot.class.hpp"
#include <ctime>

#define MAX_CLIENTS 3

extern bool	exitServer;

Server::Server() 
{
	_boat = new Bot(this);
}

void Server::disconnectClient(Client *client)
{
	close(client->getSocket());
	for (int i = 0; i < (int)this->_clients.size(); i++)
	{
		if (client == this->_clients.at(i))
		{
			this->_clients.erase(this->_clients.begin() + i);
			this->_pollFds.erase(this->_pollFds.begin() + i);
		}
	}
}

void Server::disconnectBot(Client *bot)
{
	for (int i = 0; i < (int)this->_clients.size(); i++)
	{
		if (bot == this->_clients.at(i))
		{
			this->_clients.erase(this->_clients.begin() + i);
		}
	}
}

Server::~Server()
{
	while (_clients.size() > (size_t) 0)
	{
		delete _clients.at(0);
	}
	for (int i = 0; i < (int) _channels.size(); i++)
		delete _channels.at(i);
	for (int i = 0; i < (int) _pollFds.size(); i++)
		close(_pollFds.at(i).fd);
	if (_serverPollFd.fd > 2)
		close(_serverPollFd.fd);
	if (this->_socket > 2)
		close(this->_socket);
}

Server::Server(std::string password, unsigned int port)
{
	_boat = new Bot(this); 
	this->_serverPollFd.fd = -1;
	this->_socket = -1;
	this->_name = "ft_irc";
	this->_port = port;
	this->_password = password;
	this->_operatorPassword = OPER_PASSWORD;
	if (DEBUG)
		std::cout << "Constructor called" << std::endl;
}

Server & Server::operator=(const Server & rhs)
{
	if (this != &rhs)
	{
		this->_serverPollFd.fd = -1;
		this->_socket = -1;
		this->_password = rhs._password;
		this->_port = rhs._port;
		this->_name = rhs._name;
		this->_clients = rhs._clients;
		this->_operatorPassword = rhs._operatorPassword;
	}
	return *this;
}

Server::Server(const Server & rhs)
{
	if (this != &rhs)
		*this = rhs;
}

const std::string	& Server::getName() const
{
	return (_name);
}

std::vector<std::string> Server::split(const std::string &s, const std::string &delimiter) {
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = s.find(delimiter);
    while (end != std::string::npos) {
        tokens.push_back(s.substr(start, end - start + delimiter.length()));
        start = end + delimiter.length();
        end = s.find(delimiter, start);
    }
    tokens.push_back(s.substr(start, end) + delimiter);
    return tokens;
}

void Server::handleClientMessage(size_t clientIndex)
{
    char buffer[1024];
    ssize_t messageLength = recv(this->_pollFds[clientIndex + 1].fd, buffer, sizeof(buffer) - 1, 0);
    if (messageLength == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            // No data was available to read, it's not necessarily an error
            return;
        }
        else {
            // An actual error occurred
			if (DEBUG)
            	std::cerr << "Failed to receive client message. Error: " << strerror(errno) << std::endl;
            return;
        }
    } else if (messageLength == 0) {
        Client *cl = this->_clients.at(clientIndex + 1);
        delete cl;
        return;
    }
    buffer[messageLength] = '\0';
    std::string message(buffer);
	size_t found = message.find("\r\n");
	if (found != std::string::npos)
	{
		std::string delimiter = "\r\n";
    	std::vector<std::string> tokens = this->split(message, delimiter);
		if (message == "\r\n" && this->_clients[clientIndex + 1]->getBuffReceive().length() > 0)
		{
			message = this->_clients[clientIndex + 1]->getBuffReceive() + "\r\n";
			this->_clients[clientIndex + 1]->setBuffReceive("");
			if (DEBUG)
				std::cout << "\033[31m" << "message recu from  " << this->getClient(clientIndex + 1)->getNickname() << "\n" << message << "\n\033[0m";//test
			Message mess;
			try
			{
				mess = Message(this->_clients[clientIndex + 1], message);
				if (DEBUG)
					std::cout << "Trying to execute command : " << mess.getCommand() << std::endl;
				this->_clients[clientIndex + 1]->executeCommand(mess);
			}
			catch(const std::exception& e)
			{
				if (DEBUG)
					std::cerr << e.what() << "\n";
			}
		}
    	else for (size_t i = 0; i < tokens.size() && tokens[i].length() > 0 && tokens[i] != "\r\n"; i++) 
		{
			if (this->_clients[clientIndex + 1]->getBuffReceive().length() > 0 )
			{
				this->_clients[clientIndex + 1]->appendBuffReceive(tokens[i]);
				tokens[i] = this->_clients[clientIndex + 1]->getBuffReceive();
				this->_clients[clientIndex + 1]->setBuffReceive("");
			}
			if (DEBUG)
        		std::cout << "\033[31m" << "message recu from  " << this->getClient(clientIndex + 1)->getNickname() << "\n" << tokens[i] << "\n\033[0m";//test
			Message mess;
			try
			{
				mess = Message(this->_clients[clientIndex + 1], tokens[i]);
				if (DEBUG)
					std::cout << "Trying to execute command : " << mess.getCommand() << std::endl;
				this->_clients[clientIndex + 1]->executeCommand(mess);
			}
			catch(const std::exception& e)
			{
				if (DEBUG)
					std::cerr << e.what() << "\n";
			}
    	}
	}
	else
	{
		if (this->_clients[clientIndex + 1]->getBuffReceive().length() == 0 )
			this->_clients[clientIndex + 1]->setBuffReceive(message);
		else
			this->_clients[clientIndex + 1]->appendBuffReceive(message);
		if (DEBUG)
			std::cout << "buffer contains : " << this->_clients[clientIndex + 1]->getBuffReceive() << std::endl;//test
	}
}

std::vector<std::string> Server::split(const std::string &s, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

void Server::handleNewConnection()
{
	sockaddr_in clientAddress;
	memset(&clientAddress, 0, sizeof(clientAddress));
    socklen_t clientAddressLength = sizeof(clientAddress);
    int clientSocket = accept(this->getSocket(), (struct sockaddr *)&clientAddress, &clientAddressLength);
    if (clientSocket == -1) {
        std::cerr << "Failed to accept client connection." << std::endl;
        return;
    }
    fcntl(clientSocket, F_SETFL, O_NONBLOCK);
	if (this->_clients.size() > MAX_CLIENTS)
	{
		std::string welcomeMessage = "too much clients already connected\n";
		send(clientSocket, welcomeMessage.c_str(), welcomeMessage.size(), 0);
		close(clientSocket);
		return;
	}
	std::string welcomeMessage = "Connecting....\n";
	send(clientSocket, welcomeMessage.c_str(), welcomeMessage.size(), 0);
    pollfd clientPollFd;
    clientPollFd.fd = clientSocket;
    clientPollFd.events = POLLIN;
	clientPollFd.revents = 0;
    this->_pollFds.push_back(clientPollFd);
    char ipString[INET_ADDRSTRLEN];
	if (inet_ntop(AF_INET, &(clientAddress.sin_addr), ipString, INET_ADDRSTRLEN))
	{
		Client *newClient = new Client(this, ipString);
		newClient->setSocket(clientSocket);
		this->_clients.push_back(newClient);
	}
}

void Server::inactivityCheck(std::time_t & lastPingTime)
{
	std::time_t currentTime = std::time(0);
	if ((currentTime - lastPingTime) >= 60)
	{
		for (size_t i = 1; i < this->_clients.size(); ++i)
		{
			if (this->getClient(i)->getDownTime() > 120)
			{
				std::cout << this->getClient(i)->getNickname() << " disconnected after " << this->getClient(i)->getDownTime() <<  "secondes of inactivity \n";
				std::string disconnectionMessage = "You have been disconnected after lack of response to pings during more than 2minutes";
				send(this->getClient(i)->getSocket(), disconnectionMessage.c_str(), disconnectionMessage.size(), 0);
                delete this->getClient(i);
			}
			else if (this->getClient(i)->getDownTime() >= 60)
			{
				std::string message = "PING :ft_irc\r\n";
				send(this->getClient(i)->getSocket(), message.c_str(), message.size(), 0);
			}
		}
		lastPingTime = currentTime;
	}
}

int Server::mainLoop(std::time_t & lastPingTime)
{
	this->inactivityCheck(lastPingTime);
	if (poll(this->_pollFds.data(), this->_pollFds.size(), 300) == -1)
	{
		if (exitServer)
			return (-1);
		std::cerr << "Failed to poll sockets." << std::endl;
		return (-1);
	}
	for (size_t i = 0; i < this->_pollFds.size(); ++i)
	{
        /* std::cout << "Current list of Clients conencted : "<< std::endl;
        for (int i = 0 ; i < (int) this->getClientsSize(); i++)
        {
            std::cout << "Client " << this->getClient(i)->getNickname() << std::endl;
        } */
		if (this->_pollFds[i].revents & POLLIN)
		{
			if (this->_pollFds[i].fd == this->getSocket())
				handleNewConnection();
			else
			{
				handleClientMessage(i - 1);
			}
		}
	}
	return 0;
}

int Server::serverInit()
{
	this->_clients.push_back(_boat);
	this->setSocket(socket(AF_INET, SOCK_STREAM, 0));
	if (this->getSocket() == -1)
	{
		std::cerr << "Creation of server socket failed" << std::endl;
	}
	this->_serverAddress.sin_family = AF_INET;
	this->_serverAddress.sin_addr.s_addr = INADDR_ANY;
	this->_serverAddress.sin_port = htons(this->_port);
	fcntl(this->getSocket(), F_SETFL, O_NONBLOCK);

	int reuseAddr = 1;
    if (setsockopt(this->getSocket(), SOL_SOCKET, SO_REUSEADDR, &reuseAddr, sizeof(reuseAddr)) == -1)
    {
        std::cerr << "Failed to set SO_REUSEADDR option" << std::endl;
        close(this->getSocket());
        return 1;
    }
	if (bind(this->getSocket(), reinterpret_cast<struct sockaddr*>(&this->_serverAddress), sizeof(this->_serverAddress)) == -1)
	{
		std::cerr << "Failed to bind server socket. Port may be unavailable.\n Binding to port numbers below 1024 often requires administrative privileges" << std::endl;
    	close(this->getSocket());
    	return 1;
	}

	if (listen(this->getSocket(), 0) == -1)
	{
		std::cerr << "Failed to start listening on server socket" << std::endl;
    	close(this->getSocket());	
    	return 1;
	}
	else
		std::cout << "Server listenning on port " << this->_port << std::endl;
	
	this->_serverPollFd.fd = this->getSocket();
	this->_serverPollFd.events = POLLIN;
	this->_serverPollFd.revents = 0;
	this->_pollFds.push_back(this->_serverPollFd);
	return 0;
}

std::string Server::getPassword() const
{
	return this->_password;
}

void Server::setPassword(std::string & password)
{
	this->_password = password;
}

unsigned int Server::getPort() const
{
	return this->_port;
}

void Server::setPort(unsigned int port)
{
	this->_port = port;
}

const std::string & Server::getOperatorPassword() const
{
	return (_operatorPassword);
}

int Server::getSocket() const
{
	return this->_socket;
}

void Server::setSocket(int socket)
{
	this->_socket = socket;
}

void			Server::addChannel(Channel *newChannel)
{
	_channels.push_back(newChannel);
}

Channel			*Server::getChannel(const std::string &channelName)
{
	for (int i = 0; i < (int) _channels.size(); i++)
	{
		if (_channels.at(i)->getName() == channelName)
			return (_channels.at(i)); 
	}
	return (NULL);
}

Client			*Server::getClient(int index) const
{
	return (_clients.at(index));
}

Client				*Server::getClientByNick(const std::string& nickname) const
{
    for (int i = 0; i < (int) _clients.size(); i++)
    {
        if (_clients.at(i)->getNickname() == nickname)
			return (_clients.at(i)); 
    }
    return (NULL);
}

size_t			Server::getClientsSize() const
{
	return (_clients.size());
}

std::vector<Channel *>	Server::getChannelsByMask(const std::string & mask) const
{
	std::vector<Channel *>	matches;

	for (int i = 0; i < (int) _channels.size(); i++)
	{
		if ("#" + _channels.at(i)->getName() == mask)
			matches.push_back(_channels.at(i));
	}
	return (matches);
}

std::vector<Client *>	Server::getClientsByMask(const std::string & mask) const
{
	std::vector<Client *>	matches;

	for (int i = 0; i < (int) _clients.size(); i++)
	{
		if (mask.at(0) == '$')
		{
			if ("$" + this->getName() == mask)
				matches.push_back(_clients.at(i));
		}
		else if ("#" + _clients.at(i)->getHostname() == mask)
			matches.push_back(_clients.at(i));
	}
	return (matches);
}

Client	*Server::getBoat()
{
	return (_boat);
}