#include "../../includes/Numeric.hpp"

namespace Reply{    
    const std::string welcome(const std::string &userPrefix)
    {
        return (":Welcome to the Internet Relay Network " + userPrefix );
    }

    // const std::string test(const std::string &userPrefix)
    // {
    //     return (":test " + userPrefix );
    // }
	
    const std::string yourhost(const std::string &servername, const std::string &version)
    {
        return ( ":Your host is " + servername + ", running version " + version);
    }

    const std::string created(const std::string &date)
    {
        return (":This server was created " + date);
    }

    const std::string myinfo(const std::string &servername, const std::string &version, const std::string &availableUserModes, const std::string &availableChannelModes)
    {
        return (':' + servername + ' ' + version + ' ' + availableUserModes + ' ' + availableChannelModes);
    }

    const std::string bounce(const std::string &servername, const std::string &port)
    {
        return ( ":Try server " + servername + ", port " + port);
    }

    const std::string statslinkinfo(const std::string &linkname, size_t sendq, size_t sentMessages, size_t sentKbytes, size_t recvMessages, size_t recvKbytes, time_t timeOpen)
    {
        std::stringstream ss;
        ss << linkname << ' ' << sendq << ' ' << sentMessages << ' ' << sentKbytes
        << ' ' << recvMessages << ' ' << recvKbytes << ' ' << timeOpen;
        return (ss.str());
    }

    const std::string statscommands(const std::string &command, size_t count, size_t byteCount, size_t remoteCount)
    {
        std::stringstream ss;
        ss << command << ' ' << count << ' ' << byteCount << ' ' << remoteCount;
        return (ss.str());
    }

    const std::string endofstats(const std::string &letter)
    {
        return (letter + " :End of STATS report");
    }

    const std::string umodeis(const std::string &modes)
    {
        return ( '+' + modes);
    }

    const std::string servlist(const std::string &name, const std::string &server, const std::string &mask, const std::string &type, size_t hopcount, const std::string &info)
    {
        std::stringstream ss;
        ss << name << ' ' << server << ' ' << mask << ' ' << type << ' ' << hopcount << ' ' << info;
        return (ss.str());
    }

    const std::string servlistend(const std::string &mask, const std::string &type)
    {
        return ( mask + ' ' + type + " :End of service listing");
    }

    /*
    const std::string statuptime(time_t uptime)
    {
        std::stringstream ss;
        time_t m = 60, h = 60 * m, d = 24 * h;
        ss << ":Server Up " << (uptime / d) << " days ";
        uptime %= d;
        ss << (uptime / h);
        uptime %= h;
        ss << ':' << std::setfill('0') << std::setw(2) << (uptime / m)
        << ':' << std::setfill('0') << std::setw(2) << (uptime % m);
        return (ss.str());
    }
    */
    const std::string statoline(const std::string &hostmask, const std::string &name)
    {
        return ( "O " + hostmask + " * " + name);
    }

    const std::string away(const std::string &nickname, const std::string &reason)
    {
        return (nickname + " :" + reason);
    }

    const std::string unaway()
    {
        return (":You are no longer marked as being away");
    }

    const std::string nowaway()
    {
        return (":You have been marked as being away");
    }

    const std::string list(const std::string &channel, size_t visible, const std::string &topic)
    {
        std::stringstream ss;
        ss << channel << ' ' << visible << ' ' << topic;
        return (ss.str());
    }

    const std::string listend()
    {
        return (":End of LIST");
    }

    const std::string channelmodeis(const std::string &channel, const std::string &modes)
    {
        return ( channel + " +" + modes);
    }

    const std::string notopic(const std::string &channel)
    {
        return (channel + " :No topic is set");
    }

    const std::string topic(const std::string &channel, const std::string &topic)
    {
        return (channel + " :" + topic);
    }

    const std::string inviting(const std::string &channel, const std::string &nickname)
    {
        return (channel + ' ' + nickname);
    }

    const std::string invitelist(const std::string &channel, const std::string &invitemask)
    {
        return (channel + ' ' + invitemask);
    }

    const std::string endofinvitelist(const std::string &channel)
    {
        return (channel + " :End of channel invite list");
    }

    const std::string exceptlist(const std::string &channel, const std::string &exceptionmask)
    {
        return (channel + ' ' + exceptionmask);
    }

    const std::string endofexceptlist(const std::string &channel)
    {
        return (channel + " :End of channel exception list");
    }

    const std::string version(const std::string &version, const std::string debuglevel, const std::string &server, const std::string &comments)
    {
        return (version + '.' + debuglevel + ' ' + server + " :" + comments);
    }

    const std::string namreply(const std::string &names)
    {
        return (names);
    }

    const std::string endofnames(const std::string &name)
    {
        return (name + " :End of NAMES list");
    }

    const std::string banlist(const std::string &channel, const std::string &banmask)
    {
        return (channel + ' ' + banmask);
    }

    const std::string endofbanlist(const std::string &channel)
    {
        return (channel + " :End of channel ban list");
    }

    const std::string motd(const std::string &text)
    {
        return (":- " + text);
    }

    const std::string motdstart(const std::string &servername)
    {
        return (":- " + servername + " Message of the day - ");
    }

    const std::string endofmotd()
    {
        return (":End of MOTD command");
    }

    const std::string info(const std::string &info)
    {
        return (':' + info);
    }

    const std::string endofinfo()
    {
        return (":End of INFO list");
    }

    const std::string youreoper()
    {
        return (":You are now an IRC operator");
    }

    const std::string youreservice(const std::string &servicename)
    {
        return (":You are service " + servicename);
    }

    /* const std::string time(const std::string &servername)
    {
        return (servername + toDate(::time(NULL), " :%x - %I:%M:%S %z"));
    }
 */
    const std::string lusersclient(size_t users, size_t services, size_t servers)
    {
        std::stringstream ss;
        ss << ":There are " << users << " users and " << services << " services on " << servers << " server";
        return (ss.str());	
    }

    const std::string lusersop(size_t nb)
    {
        std::stringstream ss;
        ss << nb << " :operator(s) online";
        return (ss.str());
    }

    const std::string lusersunknown(size_t nb)
    {
        std::stringstream ss;
        ss << nb << " :unknown connection(s)";
        return (ss.str());
    }

    const std::string luserschannels(size_t nb)
    {
        std::stringstream ss;
        ss << nb << " :channels formed";
        return (ss.str());
    }

    const std::string lusersme(size_t nb)
    {
        std::stringstream ss;
        ss << ":I have " << nb << " clients and 1 server";
        return (ss.str());
    }

    const std::string whoisuser(const std::string &nick, const std::string &user, const std::string &host, const std::string &realname)
    {
        return (nick + " " + user + " " + host + " * :" + realname);
    }

    const std::string whoisserver(const std::string &nick, const std::string &servername, const std::string shortinfo)
    {
        return (nick + " " + servername + " :" + shortinfo);
    }

    const std::string whoisoperator(const std::string &nick)
    {
        return (nick + " :is an IRC operator");
    }

    const std::string whoisidle(const std::string &nick, size_t idle)
    {
        std::stringstream ss;
        ss << nick << " " << idle << " :seconds idle";
        return (ss.str());
    }

    const std::string whoischannels(const std::string &nick, const std::string &channels)
    {
        return (nick + " :" + channels);
    }

    const std::string endofwhois(const std::string &nick)
    {
        return (nick + " :End of WHOIS list");
    }

    const std::string whoreply(const std::string &msg)
    {
        return (msg);
    }

    const std::string endofwho(const std::string &target)
    {
        return (target + " :End of WHO list");
    }

    const std::string whowasuser(const std::string &nick, const std::string &user, const std::string &host, const std::string &realname)
    {
        return (nick + " " + user + " " + host + " * :" + realname);
    }

    const std::string endofwhowas(const std::string &nick)
    {
        return (nick + " :End of WHOWAS");
    }

    const std::string rehashing(const std::string &filename)
    {
        return (filename + " :Rehashing");
    }

    const std::string userhostreply(const std::string &msg)
    {
        return (":" + msg);
    }
}