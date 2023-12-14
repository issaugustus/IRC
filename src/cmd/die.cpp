#include "Numeric.hpp"
#include "Client.class.hpp"
#include "Channel.class.hpp"
#include "Server.class.hpp"
#include <iostream>
#include <sstream>
#include <stdlib.h>

extern bool exitServer;

int Client::die(const Message& msg)
{
    (void)msg;
    if (!this->getIsOperator())
    {
        sendReply(ERR_NOPRIVILEGES, Error::noprivileges());
        return (-1);
    }
    exitServer = true;
    return (0);
}