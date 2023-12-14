#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include "Server.class.hpp"
#include "Client.class.hpp"
#include "Channel.class.hpp"
#include "Message.class.hpp"

bool	exitServer = false;

static bool is_digits(const std::string &port_string)
{
    return port_string.find_first_not_of("0123456789") == std::string::npos;
}

int args_error_check(std::string port_string,unsigned int *port, std::string password)
{
	if (!is_digits(port_string))
	{
		std::cerr << "wrong port format. Only unsigned integers between 1 and 65535 are allowed" << std::endl;
		return (1);
	}
    std::istringstream iss(port_string);
	if (!(iss >> *port) || *port == 0 || *port > 65535)
	{
		std::cerr << "Port must be an unsigned integer between 1 and 65535" << std::endl;
		return (1);
	}
	if (password.size() == 0)
	{
		if (DEBUG)
			std::cerr << "You must have a password" << std::endl;
		return 1;
	}
	return (0);
}

void	sigint_handler(int signum)
{
	(void) signum;
	std::cout << "Exiting server" << std::endl;
	exitServer = true;
}

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cerr << "Wrong usage. Correct is : ./ircserv <port> <password>";
		return (1);
	}
	std::string		port_string = av[1];
	std::string		password = av[2];
	unsigned int	port;
	std::time_t 	lastPingTime = std::time(0);

	signal(SIGINT, sigint_handler);
	initializeCommandMap();
	if (args_error_check(port_string, &port, password) != 0)
		return 1;
	Server *server;
	server =  new Server(password, port);
	if (server->serverInit() != 0)
	{
		delete server;
		return 1;
	}
	else
	{
		exitServer = false;
		while (!exitServer)
			server->mainLoop(lastPingTime);
	}
	delete server;
	return (0);
}