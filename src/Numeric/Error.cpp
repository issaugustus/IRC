#include "../../includes/Numeric.hpp"

//rajouter les constantes numeriques apres avoir check le format message
//suffit de rajouter ERR_nomdelafonction en maj pour avoir la numeric value

namespace Error {
    const std::string   nosuchnick(const std::string& nickname)
    {
        return (nickname + " :No such nick/channel");
    }

    const std::string	nosuchserver(const std::string& servername)
    {
        return (servername + " :No such server");
    }

    const std::string	nosuchchannel(const std::string& channelname)
    {
        return (channelname + " :No such channel");
    }

    const std::string	cannotsendtochan(const std::string &channelname)
    {
        return (channelname + " :Cannot send to channel");
    }

    const std::string	toomanychannels(const std::string &channelname)
    {
        return (channelname + " :You have joined too many channels");
    }

    const std::string 	wasnosuchnick(const std::string &nickname)
    {
        return (nickname + " :There was no such nickname");
    }

    const std::string	toomanytargets(const std::string &target)
    {
        return (target + " :Too many targets, message not delivered");
    }

    const std::string	nosuchservice(const std::string &servicename)
    {
        return (servicename + " :No such service");
    }

    const std::string	noorigin()
    {
        return (" :No origin specified");
    }

    const std::string	norecipient(const std::string &command)
    {
        return (":No recipient given for " + command);
    }

    const std::string	notexttosend()
    {
        return (":No text to send");
    }

    const std::string	notoplevel(const std::string &mask)
    {
        return (mask + " :No top level domain specified");
    }

    const std::string	wildtoplevel(const std::string &mask)
    {
        return (mask + " :Wildcard in top level domain");
    }

    const std::string	badmask(const std::string &mask)
    {
        return (mask + " :Bad server/host mask");
    }

    const std::string	unknowncommand(const std::string &command)
    {
        return (command + " :Unknown command");
    }

    const std::string	nomotd()
    {
        return (":MOTD file is missing");
    }

    const std::string	noadmininfo()
    {
        return (":No administrative info available");
    }

    const std::string	fillerror(const std::string& file_op, const std::string& file)
    {
        return (":File error doing " + file_op + " :on " + file);
    }

    const std::string	nonicknamegiven()
    {
        return (":No nickname given");
    }

    const std::string	erroneusnickname(const std::string &nickname)
    {
        return (nickname + " :Erroneus nickname");
    }

    const std::string	nicknameinuse(const std::string& nickname)
    {
        return (nickname + " :Nickname is already in use");
    }

    const std::string	nickcollision(const std::string& nickname)
    {
        return (nickname + " :Nickname collision kill");
    }

    const std::string	usernotinchannel(const std::string& nickname, const std::string& channel)
    {
        return (nickname + ' ' + channel + " :Not in that channel");
    }

    const std::string	notonchannel(const std::string& channel)
    {
        return (channel + " :You're not on that channel");
    }

    const std::string	useronchannel(const std::string& nickname, const std::string& channel)
    {
        return (nickname + ' ' + channel + " :is already on channel");
    }

    const std::string	nologin(const std::string& user)
    {
        return (user + " :User is not logged in");
    }

    const std::string	summondisabled()
    {
        return (":Summon has been disabled");
    }

    const std::string	usersdisabled()
    {
        return (":USERS has been disabled");
    }

    const std::string	notregistered()
    {
        return (":You have not registered yet");
    }

    const std::string	needmoreparams(const std::string& command)
    {
        return (command + " :Not enough parameters");
    }

    const std::string	alreadyregistered()
    {
        return (":Unauthorized command (already registered)");
    }

    const std::string	nopermforhost()
    {
        return (":Your host is not among the privileged");
    }

    const std::string	passwdmissmatch()
    {
        return (":Password incorrect. Connection to server failed");
    }

    const std::string	yourebannedcreep()
    {
        return ("You are banned from this server");
    }

    const std::string	keyset(const std::string& channel)
    {
        return (channel + " :Channel key already set");
    }

    const std::string	channelisfull(const std::string& channel)
    {
        return (channel + " :Channel is full");
    }

    const std::string	unknownmode(const std::string &c)
    {
        return (c + ":is unknown mode char to me");
    }

    const std::string	inviteonlychan(const std::string& channel)
    {
        return (channel + " :Cannot join channel (+i)");
    }

    const std::string	bannedfromchan(const std::string& channel)
    {
        return (channel + " :Cannot join channel (+b)");
    }

    const std::string	badchannelkey(const std::string& channel)
    {
        return (channel + " :Cannot join channel (+k)");
    }

    const std::string	nochanmodes(const std::string& channel)
    {
        return (channel + " :Channel doesn't support modes");
    }

    const std::string	noprivileges()
    {
        return (":Permission denied - You're not an IRC operator");
    }

    const std::string	chanoprisneeded(const std::string& channel)
    {
        return (channel + " :You're not a channel operator");
    }

    const std::string	cantkillserver()
    {
        return (":You can't kill a server");
    }

    const std::string	restricted()
    {
        return (":Your connection is restricted");
    }

    const std::string	nooperhost()
    {
        return (":No O-lines for your host");
    }

    const std::string	umodeunknownflag()
    {
        return (":Unknown MODE flag");
    }

    const std::string	usersdontmatch()
    {
        return (":Can't change mode for other users");
    }
}