#include "../../includes/Numeric.hpp"
#include "../../includes/Client.class.hpp"
#include "../../includes/Channel.class.hpp"
#include "../../includes/Server.class.hpp"
#include "../../includes/ErrorCode.hpp"

int	Client::kill(const Message & msg)
{
	Client	*toKill;
	if (msg.getParamSize() < (size_t) 1)
	{
		sendReply(ERR_NEEDMOREPARAMS, Error::needmoreparams(msg.getCommand()));
		return (-1);
	}
	toKill = _server->getClientByNick(msg.getParam(0));
	if (!toKill)
	{
		sendReply(ERR_NOSUCHNICK, Error::nosuchnick(msg.getParam(0)));
		return (-1);
	}
	if (!_isOperator)
	{
		sendReply(ERR_NOPRIVILEGES, Error::noprivileges());
		return (-1);
	}
	if (toKill != _server->getBoat())
		delete toKill;
	return (0);
}