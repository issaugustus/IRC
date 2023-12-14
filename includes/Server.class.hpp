#ifndef SERVER_CLASS_HPP
# define SERVER_CLASS_HPP
#include "Client.class.hpp"
#include "Channel.class.hpp"
#include "Bot.class.hpp"
#include <cstring>
#include <arpa/inet.h>
#include <vector>
#include <sys/poll.h>

# define OPER_PASSWORD "lemotdepasse"

class Client;
class Channel;
class Bot;
class Server
{
	public :
		Server(std::string password, unsigned int port);
		Server(const Server & rhs);
		Server & operator=(const Server & rhs);
		~Server();
		
		int	serverInit();
		int mainLoop(std::time_t & lastPingTime);
		void handleNewConnection();
		void handleClientMessage(size_t clientIndex);

		std::vector<std::string> split(const std::string &s, const std::string &delimiter);

		void				inactivityCheck(std::time_t & lastPingTime);
		void				disconnectClient(Client *client);
		void 				disconnectBot(Client *bot);
		const std::string &	getName() const;
		in_addr_t 			getIpAddress() const;
		std::string 		getPassword() const;
		void				setPassword(std::string & password);
		unsigned int		getPort() const;
		int					getSocket() const;
		const std::string & getOperatorPassword() const;
		Channel				*getChannel(const std::string &channelName);
		Client				*getClient(int index) const;
        Client				*getClientByNick(const std::string& nickname) const;
		size_t				getClientsSize() const;

		void				setPort(unsigned int port);
		void				setSocket(int socket);
		void 				setIpAddress(in_addr_t & ipAddress);

		std::vector<Channel *>	getChannelsByMask(const std::string & mask) const;
		std::vector<Client *>	getClientsByMask(const std::string & mask) const;

		std::vector<std::string> split(const std::string &s, char delimiter);

		void				addChannel(Channel *newChannel);

		Client				*getBoat();

	private : 
		int									_socket;
		Server();
		std::string							_password;
		std::string							_operatorPassword;
		std::string							_name;
		unsigned int						_port;
		struct pollfd						_serverPollFd;
		struct sockaddr_in 					_serverAddress;
		std::vector<Client *>				_clients;
		std::vector<Channel *>				_channels;
		std::vector<struct pollfd>			_pollFds;
		std::vector<struct sockaddr_in>		_clientAddresses;
		Bot									*_boat;
};

#endif