
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "Client.class.hpp"
#include "Server.class.hpp"

class Client;
class Server;
class Channel
{
	public :
		Channel(Server *server, const std::string & name);
		Channel(Server *server, const std::string & name, const std::string & password);
		Channel(const Channel & rhs);
		Channel &	operator=(const Channel & rhs);
		~Channel();
	
		bool				isInviteOnly() const;
		void				setInviteOnly(bool value);

		const std::string &	getTopic() const;
		void				setTopic(const std::string & topic);

		int					getUserLimit() const;
		void				setUserLimit(int limit);

		bool				hasPassword() const;
		const std::string &	getPassword() const;
		void				setPassword(const std::string & password);
		void				removePassword();

		bool				isOperator(Client *client) const;
		bool				isOperator(const std::string & nickname) const;
		Client				*getOperator(int index) const;
		Client				*getOperator(const std::string & nickname) const;
		size_t				getNbOperator() const;
		void				addOperator(Client *chanop);
		void				removeOperator(int index);
		void				removeOperator(Client *chanop);

		bool				isMember(Client *client) const;
		bool				isMember(const std::string & nickname) const;

		bool				isBanned(Client *client) const;
		bool				isBanned(const std::string & nickname) const;

		void				banMember(Client *client);
		void				banMember(const std::string & nickname);

		void				unbanMember(Client *client);
		void				unbanMember(const std::string & nickname);

		Client				*getMember(const std::string & nickname) const;
		Client				*getMember(int index) const;
		size_t				getNbMember() const;
		void				addMember(Client * member);
		void				removeMember(int index);
		void				removeMember(Client *member);
	
		bool				hasMutableTopic() const;
		void				setMutableTopic(bool value);

		void				sendMessage();

		void				invite(Client *client);
		bool				isInvited(Client *client) const;
		void				removeInvite(Client *client);

		const std::string &		getName() const;

		class	invalidNameException : public std::exception
		{
			public:
				virtual const char	*what() const throw();
		};
		class	emptyPasswordException : public std::exception
		{
			public:
				virtual const char	*what() const throw();
		};

		class	noPasswordException : public std::exception
		{
			public:
				virtual const char	*what() const throw();
		};

	private :
		Channel();
		Server					*_server;
		std::string				_topic;
		std::vector<Client *>	_operators;
		std::vector<Client *>	_members;
		std::vector<Client *>	_invited;
		std::vector<Client *>	_banned;
		std::string				_name;
		bool					_hasPassword;
		std::string				_password;
		bool					_mutableTopic;
		bool					_inviteOnly;
		int						_userLimit;
};