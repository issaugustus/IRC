#pragma once
#include <cstring>
#include <string>
#include <vector>
#include <map>

class Client;
void	initializeCommandMap();
class Message
{
	public:
		Message(Client *sender, const std::string & content);
		Message(Client *sender, const char *content);
		Message(const Message & rhs);
		Message & operator=(const Message & rhs);
		~Message();
		const std::string & getParam(int index) const;
		const std::string &	getPrefix() const;
		const std::string &	getCommand() const;
		const std::string &	getParameters() const;
		const Client	*getSender() const;
		size_t	getParamSize() const;
		//bool	isValid() const;
		typedef int (Client::*functionPointer)(const Message &);
		static std::map<std::string, functionPointer>	commandMap;
		static std::map<std::string, functionPointer>	unregisteredCommands;

		class AMessageComponent
		{
			public:
				AMessageComponent();
				AMessageComponent(const AMessageComponent & rhs);
				AMessageComponent & operator=(const AMessageComponent & rhs);
				virtual ~AMessageComponent() = 0;
				bool	isEmpty() const;
				const 	std::string &	getContent() const;
			protected:
				std::string	_content;
				bool		_isEmpty;
		};

		class Prefix : public AMessageComponent
		{
			public:
				Prefix() : AMessageComponent() {}
				Prefix(const std::string & messageContent);
				Prefix(const Prefix & rhs);
				Prefix & operator=(const Prefix & rhs);
				~Prefix();
				
			private:
				bool	_isNick; // true : le prefixe est un nick, false : le prefix est un servername
				bool	_hasUser;
				bool	_hasHost;
				std::string	_pfx;
				std::string	_user;
				std::string	_host;
		};

		class Params : public AMessageComponent
		{
			public:
				Params() : AMessageComponent() {}
				Params(const std::string & messageContent);
				Params(const Params & rhs);
				Params & operator=(const Params & rhs);
				~Params();
				bool	isValid() const;
				const std::string & getParam(int index) const;
				size_t	getSize() const;
			private:
				std::vector<std::string>	_parameters;
		};

		class Command : public AMessageComponent
		{
			public:
				Command() : AMessageComponent() {}
				Command(const std::string & messageContent);
				Command(const Command & rhs);
				Command & operator=(const Command & rhs);
				~Command();
				bool	isValid() const;
				//bool	isValidParams(const Message::Params & params) const;
		};

		class	InvalidMessageException : public std::exception
		{
			public:
				virtual const char	*what() const throw();
		};


		class  InvalidCommandException : public std::exception
		{
			public:
				InvalidCommandException(char *msg);
				InvalidCommandException(const char *msg);
				InvalidCommandException(std::string &msg);
				virtual const char *what() const throw();
				~InvalidCommandException() throw() {};
			private:
				std::string	_msg;
		};

		Message();
	private:
		std::string	_content;
		Prefix		_prefix;
		Command		_command;
		Params		_params;
		Client		*_sender;
};