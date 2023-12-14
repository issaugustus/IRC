#pragma once
#include "Client.class.hpp"
#include "Numeric.hpp"
#include <cstring>

# define OPER_PASSWORD "lemotdepasse"

class Server;
class Bot : public Client
{
	public:
		Bot();
		Bot(Server *server);
		Bot(const Bot & rhs);
		Bot & operator=(const Bot & rhs);
		~Bot();
		void		sendReply(NumericCode num, const std::string & reply);
		void		send_buff(const std::string& message);
};