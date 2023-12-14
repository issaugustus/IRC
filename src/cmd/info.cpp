#include "Client.class.hpp"
#include "Message.class.hpp"
#include "Numeric.hpp"

int					Client::info(const Message& msg)
{
	if (msg.getCommand() != "INFO")
		return (1);
	else if ((int) msg.getParamSize() == 0)
	{
		sendReply(RPL_INFO, Reply::info("FT_IRC server by kharfach, ael-youb and eloevenb"));
		sendReply(RPL_INFO, Reply::info("Reference client : irssi"));
		sendReply(RPL_ENDOFINFO,Reply::endofinfo());
	}
	else
		sendReply(ERR_NOSUCHSERVER,Error::nosuchserver(msg.getParam(0)));
	return (0);
}