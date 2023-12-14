#pragma once
#include <string>
#include <sstream>

#define DEBUG 1
enum NumericCode
{
	ERR_NOSUCHNICK = 401,
	ERR_NOSUCHSERVER = 402,
	ERR_NOSUCHCHANNEL = 403,
	ERR_CANNOTSENDTOCHAN = 404,
	ERR_TOOMANYCHANNELS = 405,
	ERR_WASNOSUCHNICK = 406,
	ERR_TOOMANYTARGETS = 407,
	ERR_NOSUCHSERVICE = 408,
	ERR_NOORIGIN = 409,
	ERR_NORECIPIENT = 411,
	ERR_NOTEXTTOSEND = 412,
	ERR_NOTOPLEVEL = 413,
	ERR_WILDTOPLEVEL = 414,
	ERR_BADMASK = 415,
	ERR_UNKNOWNCOMMAND = 421,
	ERR_NOMOTD = 422,
	ERR_NOADMINFO = 423,
	ERR_FILERROR = 424,
	ERR_NONICKNAMEGIVEN = 431,
	ERR_ERRONEUSNICKNAME = 432,
	ERR_NICKNAMEINUSE = 433,
	ERR_NICKCOLLISION = 436,
	ERR_UNAVAILABLE_RESOURCE = 437,
	ERR_USERNOTINCHANNEL = 441,
	ERR_NOTONCHANNEL = 442,
	ERR_USERONCHANNEL = 443,
	ERR_NOLOGIN = 444,
	ERR_SUMMONDISABLED = 445,
	ERR_USERDISABLED = 446,
	ERR_NOTREGISTERED = 451,
	ERR_NEEDMOREPARAMS = 461,
	ERR_ALREADYREGISTRED = 462,
	ERR_NOPERMFORHOST = 463,
	ERR_PASSWDMISSMATCH = 464,
	ERR_YOUREBANNEDCREEP = 465,
	ERR_YOUWILLBEBANNED = 466,
	ERR_KEYSET = 467,
	ERR_CHANNELISFULL = 471,
	ERR_UNKNOWNMODE = 472,
	ERR_INVITEONLYCHAN = 473,
	ERR_BANNEDFROMCHAN = 474,
	ERR_BADCHANNELKEY = 475,
	ERR_BADCHANMASK = 476,
	ERR_NOCHANMODES = 477,
	ERR_BANNLISTFULL = 478,
	ERR_NOPRIVILEGES = 481,
	ERR_CHANOPRISNEED = 482,
	ERR_CANTKILLSERVER = 483,
	ERR_RESTRICTED = 484,
	ERR_UNIQOPPRIVSNEEDED = 485,
	ERR_NOOPERHOST = 491,
	ERR_UMODEUNKNOWNFLAG = 501,
	ERR_USERSDONTMATCH = 502,

	RPL_WELCOME = 1,
	RPL_YOURHOST = 2,
	RPL_CREATED = 3,
	RPL_MYINFO = 4,
	RPL_BOUNCE = 5,
	RPL_STATSLINKINFO = 211,
	RPL_STATSCOMMANDS = 212,
	RPL_ENDOFSTATS = 219,
	RPL_UMODEIS = 221,
	RPL_SERVLIST = 234,
	RPL_SERVLISTEND = 235,
	RPL_STATSUPTIME = 242,
	RPL_STATSOLINE = 243,
	RPL_LUSERCLIENT = 251,
	RPL_LUSEROP = 252,
	RPL_LUSERUNKNOWN = 253,
	RPL_LUSERCHANNELS = 254,
	RPL_LUSERME = 255,
	RPL_AWAY = 301,
	RPL_USERHOST = 302,
	RPL_UNAWAY = 305,
	RPL_NOWAWAY = 306,
	RPL_WHOISUSER = 311,
	RPL_WHOISSERVER = 312,
	RPL_WHOISOPERATOR = 313,
	RPL_WHOISIDLE = 317,
	RPL_ENDOFWHOIS = 318,
	RPL_WHOISCHANNELS = 319,
	RPL_ENDOFWHO = 315,
	RPL_WHOWASUSER = 314,
	RPL_ENDOFWHOWAS = 369,
	RPL_LISTSTART = 321, /* Not used / (╯°□°）╯︵ ┻━┻  */
	RPL_LIST = 322,
	RPL_LISTEND = 323,
	RPL_CHANNELMODEIS = 324,
	RPL_NOTOPIC = 331,
	RPL_TOPIC = 332,
	RPL_INVITING = 341,
	RPL_INVITELIST = 346,
	RPL_ENDOFINVITELIST =  347,
	RPL_EXCEPTLIST = 348,
	RPL_ENDOFEXCEPTLIST = 349,
	RPL_VERSION = 351,
	RPL_WHOREPLY = 352,
	RPL_NAMREPLY = 353,
	RPL_ENDOFNAMES = 366,
	RPL_BANLIST = 367,
	RPL_ENDOFBANLIST = 368,
	RPL_INFO = 371,
	RPL_MOTD = 372,
	RPL_ENDOFINFO = 374,
	RPL_MOTDSTART = 375,
	RPL_ENDOFMOTD = 376,
	RPL_YOUREOPER = 381,
	RPL_REHASHING = 382,
	RPL_YOURESERVICE = 383,
	RPL_TIME = 391
};

namespace Reply
{
		const std::string welcome(const std::string &userPrefix);
		const std::string yourhost(const std::string &servername, const std::string &version);
		const std::string created(const std::string &date);
		const std::string myinfo(const std::string &servername, const std::string &version, const std::string &availableUserModes, const std::string &availableChannelModes);
		const std::string bounce(const std::string &servername, const std::string &port);
		const std::string statslinkinfo(const std::string &linkname, size_t sendq, size_t sentMessages, size_t sentKbytes, size_t recvMessages, size_t recvKbytes, time_t timeOpen);
		const std::string statscommands(const std::string &command, size_t count, size_t byteCount, size_t remoteCount);
		const std::string endofstats(const std::string &letter);
		const std::string umodeis(const std::string &modes);
		const std::string servlist(const std::string &name, const std::string &server, const std::string &mask, const std::string &type, size_t hopcount, const std::string &info);
		const std::string servlistend(const std::string &mask, const std::string &type);
		//const std::string statuptime(time_t uptime);
		const std::string statoline(const std::string &hostmask, const std::string &name);
		const std::string away(const std::string &nickname, const std::string &reason);
		const std::string unaway();
		const std::string nowaway();
		const std::string list(const std::string &channel, size_t visible, const std::string &topic);
		const std::string listend();
		const std::string channelmodeis(const std::string &channel, const std::string &modes);
		const std::string notopic(const std::string &channel);
		const std::string topic(const std::string &channel, const std::string &topic);
		const std::string inviting(const std::string &channel, const std::string &nickname);
		const std::string invitelist(const std::string &channel, const std::string &invitemask);
		const std::string endofinvitelist(const std::string &channel);
		const std::string exceptlist(const std::string &channel, const std::string &exceptionmask);
		const std::string endofexceptlist(const std::string &channel);
		const std::string version(const std::string &version, const std::string debuglevel, const std::string &server, const std::string &comments);
		const std::string namreply(const std::string &names);
		const std::string endofnames(const std::string &name);
		const std::string banlist(const std::string &channel, const std::string &banmask);
		const std::string endofbanlist(const std::string &channel);
		const std::string motd(const std::string &text);
		const std::string motdstart(const std::string &servername);
		const std::string endofmotd();
		const std::string info(const std::string &info);
		const std::string endofinfo();
		const std::string youreoper();
		const std::string youreservice(const std::string &servicename);
		const std::string time(const std::string &servername);
		const std::string lusersclient(size_t users, size_t services, size_t servers);
		const std::string lusersop(size_t nb);
		const std::string lusersunknown(size_t nb);
		const std::string luserschannels(size_t nb);
		const std::string lusersme(size_t nb);
		const std::string whoisuser(const std::string &nick, const std::string &user, const std::string &host, const std::string &realname);
		const std::string whoisserver(const std::string &nick, const std::string &servername, const std::string shortinfo);
		const std::string whoisoperator(const std::string &nick);
		const std::string whoisidle(const std::string &nick, size_t idle);
		const std::string whoischannels(const std::string &nick, const std::string &channels);
		const std::string endofwhois(const std::string &nick);
		const std::string whoreply(const std::string &msg);
		const std::string endofwho(const std::string &target);
		const std::string whowasuser(const std::string &nick, const std::string &user, const std::string &host, const std::string &realname);
		const std::string endofwhowas(const std::string &nick);
		const std::string rehashing(const std::string &filename);
		const std::string userhostreply(const std::string &msg);
}

namespace Error
{
	//rajouter les constantes numeriques apres avoir check le format message
	//suffit de rajouter ERR_nomdelafonction en maj pour avoir la numeric value
    const std::string   nosuchnick(const std::string& nickname);
    const std::string	nosuchserver(const std::string& servername);
    const std::string	nosuchchannel(const std::string& channelname);
    const std::string	cannotsendtochan(const std::string &channelname);
    const std::string	toomanychannels(const std::string &channelname);
    const std::string 	wasnosuchnick(const std::string &nickname);
    const std::string	toomanytargets(const std::string &target);
    const std::string	nosuchservice(const std::string &servicename);
    const std::string	noorigin();
    const std::string	norecipient(const std::string &command);
    const std::string	notexttosend();
    const std::string	notoplevel(const std::string &mask);
    const std::string	wildtoplevel(const std::string &mask);
    const std::string	badmask(const std::string &mask);
    const std::string	unknowncommand(const std::string &command);
    const std::string	nomotd();
    const std::string	noadmininfo();
    const std::string	fillerror(const std::string& file_op, const std::string& file);
    const std::string	nonicknamegiven();
    const std::string	erroneusnickname(const std::string &nickname);
    const std::string	nicknameinuse(const std::string& nickname);
    const std::string	nickcollision(const std::string& nickname);
    const std::string	usernotinchannel(const std::string& nickname, const std::string& channel);
    const std::string	notonchannel(const std::string& channel);
    const std::string	useronchannel(const std::string& nickname, const std::string& channel);
    const std::string	nologin(const std::string& user);
    const std::string	summondisabled();
    const std::string	usersdisabled();
    const std::string	notregistered();
    const std::string	needmoreparams(const std::string& command);
    const std::string	alreadyregistered();
    const std::string	nopermforhost();
    const std::string	passwdmissmatch();
    const std::string	yourebannedcreep();
    const std::string	keyset(const std::string& channel);
    const std::string	channelisfull(const std::string& channel);
    const std::string	unknownmode(const std::string &c);
    const std::string	inviteonlychan(const std::string& channel);
    const std::string	bannedfromchan(const std::string& channel);
    const std::string	badchannelkey(const std::string& channel);
    const std::string	nochanmodes(const std::string& channel);
    const std::string	noprivileges();
    const std::string	chanoprisneeded(const std::string& channel);
    const std::string	cantkillserver();
    const std::string	restricted();
    const std::string	nooperhost();
    const std::string	umodeunknownflag();
    const std::string	usersdontmatch();
}