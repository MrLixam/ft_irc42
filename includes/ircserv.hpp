/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvincent <lvincent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 16:23:28 by r                 #+#    #+#             */
/*   Updated: 2024/07/02 19:25:09 by lvincent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCSERV_HPP
#define IRCSERV_HPP

#include <iostream>
#include <list>
#include <string>

#define	ERR_NOSUCHNICK(errorInfo)				commandException(401, std::string(errorInfo) + std::string(" :No such nick/channel"))
#define	ERR_NOSUCHCHANNEL(errorInfo)			commandException(403, std::string(errorInfo) + std::string(" :No such channel"))
#define	ERR_CANNOTSENDTOCHAN(errorInfo)			commandException(404, std::string(errorInfo) + std::string(" :Cannot send to channel"))
#define	ERR_TOOMANYTARGETS(errorInfo)			commandException(407, std::string(errorInfo) + std::string(" :recipients. Message not delivered"))
#define ERR_NORECIPIENT(errorInfo)				commandException(411, std::string(errorInfo) + std::string(" :No recipient given"))
#define	ERR_NOTEXTTOSEND(errorInfo) 			commandException(412, std::string(errorInfo) + std::string(" :No text to send"))
#define	ERR_UNKNOWNCOMMAND(errorInfo)			commandException(421, std::string(errorInfo) + std::string(" :Unknown command"))
#define	ERR_NONICKNAMEGIVEN(errorInfo)			commandException(431, std::string(errorInfo) + std::string(" :No nickname given"))
#define	ERR_ERRONEUSNICKNAME(errorInfo)			commandException(432, std::string(errorInfo) + std::string(" :Erroneous nickname"))
#define	ERR_NICKNAMEINUSE(errorInfo)			commandException(433, std::string(errorInfo) + std::string(" :Nickname is already in use"))
#define	ERR_USERNOTINCHANNEL(errorInfo)			commandException(441, std::string(errorInfo) + std::string(" :They aren't on that channel"))
#define	ERR_NOTONCHANNEL(errorInfo)				commandException(442, std::string(errorInfo) + std::string(" :You're not on that channel"))
#define	ERR_USERONCHANNEL(errorInfo)			commandException(443, std::string(errorInfo) + std::string(" :is already on channel"))
#define	ERR_NOTREGISTERED(errorInfo)			commandException(451, std::string(errorInfo) + std::string(" :You have not registered"))
#define	ERR_NEEDMOREPARAMS(errorInfo)			commandException(461, std::string(errorInfo) + std::string(" :Not enough parameters"))
#define	ERR_ALREADYREGISTRED(errorInfo)			commandException(462, std::string(errorInfo) + std::string(" :Unauthorized command (already registered)"))
#define	ERR_PASSWDMISMATCH(errorInfo)			commandException(464, std::string(errorInfo) + std::string(" :Password incorrect"))
#define	ERR_YOUREBANNEDCREEP(errorInfo)			commandException(465, std::string(errorInfo) + std::string(" :You are banned from this server"))
#define ERR_CHANNELISFULL(errorInfo)			commandException(471, std::string(errorInfo) + std::string(" :Cannot join channel (+l)"))
#define ERR_UNKNOWNMODE(errorInfo)				commandException(472, std::string(errorInfo) + std::string(" :is unknown mode char to me"))
#define ERR_INVITEONLYCHAN(errorInfo)			commandException(473, std::string(errorInfo) + std::string(" :Cannot join channel (+i)"))
#define ERR_BADCHANNELKEY(errorInfo)			commandException(475, std::string(errorInfo) + std::string(" :Cannot join channel (+k)"))
#define ERR_CHANOPRIVSNEEDED(errorInfo)			commandException(482, std::string(errorInfo) + std::string(" :You're not channel operator"))
#define ERR_INVALIDMODEPARAM(errorInfo)			commandException(696, std::string(errorInfo))


typedef struct structured_message
{
	std::string				nickname;
	std::string				user;
	std::string				host;
	std::string				command;
	std::list<std::string>	params;
}							struct_msg;

bool						isspecial(char c);
void						format_nickname(std::string client, std::string nick);
bool						format_channel(std::string chan);
bool						format_key(std::string key);
bool						msgto_nickname(std::string nick);
struct_msg					structuring_message(std::string message);
std::string					msg_source(struct_msg msg);
int							parseArgs(char **argv);
std::vector<std::string>	parseMode(std::vector<std::string> modestring_lst);

class commandException : public std::exception
{
	public:
		commandException(int errorCode, std::string errorMessage): _errorCode(errorCode), _errorMessage(errorMessage) {}
		~commandException() throw() {};
		virtual const char *	what() const throw()
		{
			return ("sent an error code");
		}
		int						_errorCode;
		std::string				_errorMessage;
};

#endif
