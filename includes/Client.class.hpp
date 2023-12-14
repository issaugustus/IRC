#pragma once
#include <string>
#include <iostream>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/poll.h>
#include <vector>
#include <unistd.h>
#include <ctime>
#include "Message.class.hpp"
#include "Numeric.hpp"

class Server;
class Message;
class Client
{
	protected :
		Server*						_server;
		bool						_isOperator;
		bool						_isRegistered;
		bool						_isAway;
		bool						_servNotice;
		bool						_invisible;
		bool						_restricted;
		bool						_hasPassword;
		bool						_hasNickname;
		bool						_hasUsername;
        bool                        _isUp;
		std::time_t					_lastActivity;
		std::string					_buffReceive;

		std::string					_nickname;
		std::string					_name;
		std::string					_username;
		std::string					_password;
		std::string					_hostname;
		std::string					_servername;
		std::string					_awayReason;

		in_addr_t					_ipAddress;
		std::string					_awayMessage;
		std::vector<std::string>	_channels;
		int							_socket;// mettre en prive

	public :
		Client();
		Client(Server *server, const char *ipAddress); // preciser les arguments plus tard
		Client(const Client & rhs);
		Client & operator=(const Client & rhs);
		virtual ~Client();
		
        Server*             getServer() const;
		const std::string&	getUsername() const;
		const std::string&	getNickname() const;
		const std::string&	getName() const;
		const std::string&	getHostname() const;
		const std::string&	getAwayMessage() const;
		const std::string	getUserMask() const;

		const in_addr_t&	getIpAddress() const;
		const int&			getSocket() const;
		void				setSocket(int socket);


		bool				getIsOperator() const;
		bool				getIsRegistered() const;
		bool				getIsAway() const;

		int					getDownTime() const;
		void				resetDownTime();

		virtual void		send_buff(const std::string& message); //raw message

		void				join_channel(std::string& channel_name);
		void				leave_channel(std::string& channel_name);

		std::string&		getBuffReceive();
		void				setBuffReceive(std::string buff);
		void				appendBuffReceive(std::string buff);

		int					executeCommand(const Message & msg);

		void				change_nickname(std::string& nickname);
		void				set_away(std::string& away_message);
		void				set_operator_mode(std::string& password);
		void				authenticate(std::string& username, std::string& password);
		void				handle_error(int error_type, std::string& error_message);
		virtual void		sendReply(NumericCode num, const std::string & reply);
		//void			send_message(std::string& target, std::string& message);// reecrire avec nouvelles classes parsees
		//void			parse_message(std::string message);

		//====================Commands==========================================
		int					admin(const Message& msg);
		int					connect(const Message& msg);
		int					die(const Message& msg);
		int					info(const Message& msg);
		int					invite(const Message& msg);
		int					join(const Message& msg);
		int					kick(const Message& msg);
		int					kill(const Message& msg);
		int					list(const Message& msg);
		int					links(const Message& msg);
		int					lusers(const Message& msg);
		int					mode(const Message& msg);
		int					motd(const Message& msg);
		int					names(const Message& msg);
		int					nick(const Message& msg);
		int					notice(const Message& msg);
		int					oper(const Message& msg);
		int					part(const Message& msg);
		int					pass(const Message& msg);
		int					ping(const Message& msg);
		int					pong(const Message& msg);
		int					privmsg(const Message& msg);
		int					quit(const Message& msg);
		int					rehash(const Message& msg);
		int					restart(const Message& msg);
		int					server(const Message& msg);
		int					squit(const Message& msg);
		int					service(const Message& msg);
		int					servlist(const Message& msg);
		int					squery(const Message& msg);
		int					stats(const Message& msg);
		int					time(const Message& msg);
		int					topic(const Message& msg);
		int					trace(const Message& msg);
		int					user(const Message& msg);
		int					userhost(const Message& msg);
		int					version(const Message& msg);
		int					who(const Message& msg);
		int					whois(const Message& msg);
		int					whowas(const Message& msg);
};
