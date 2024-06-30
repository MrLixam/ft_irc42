/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvincent <lvincent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 16:23:28 by r                 #+#    #+#             */
/*   Updated: 2024/06/30 14:45:59 by lvincent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCSERV_HPP
#define IRCSERV_HPP

#include <iostream>
#include <list>
#include <string>

#define	ERR_NOSUCHNICK(errorInfo)				commandException(401, errorInfo + " :No such nick/channel")
#define	ERR_NOSUCHCHANNEL(errorInfo)			commandException(403, errorInfo + " :No such channel")
#define	ERR_CANNOTSENDTOCHAN(errorInfo)			commandException(404, errorInfo + " :Cannot send to channel")
#define	ERR_TOOMANYTARGETS(errorInfo)			commandException(407, errorInfo + " :recipients. Message not delivered")
#define ERR_NORECIPIENT(errorInfo)				commandException(411, errorInfo + " :No recipient given")
#define	ERR_NOTEXTTOSEND(errorInfo) 			commandException(412, errorInfo + " :No text to send")
#define	ERR_UNKNOWNCOMMAND(errorInfo)			commandException(421, errorInfo + " :Unknown command")
#define	ERR_NONICKNAMEGIVEN(errorInfo)			commandException(431, errorInfo + " :No nickname given")
#define	ERR_ERRONEUSNICKNAME(errorInfo)			commandException(432, errorInfo + " :Erroneous nickname")
#define	ERR_NICKNAMEINUSE(errorInfo)			commandException(433, errorInfo + " :Nickname is already in use")
#define	ERR_USERNOTINCHANNEL(errorInfo)			commandException(441, errorInfo + " :They aren't on that channel")
#define	ERR_NOTONCHANNEL(errorInfo)				commandException(442, errorInfo + " :You're not on that channel")
#define	ERR_USERONCHANNEL(errorInfo)			commandException(443, errorInfo + " :is already on channel")
#define	ERR_NOTREGISTERED(errorInfo)			commandException(451, errorInfo + " :You have not registered")
#define	ERR_NEEDMOREPARAMS(errorInfo)			commandException(461, errorInfo + " :Not enough parameters")
#define	ERR_ALREADYREGISTRED(errorInfo)			commandException(462, errorInfo + " :Unauthorized command (already registered)")
#define	ERR_PASSWDMISMATCH(errorInfo)			commandException(464, errorInfo + " :Password incorrect")
#define	ERR_YOUREBANNEDCREEP(errorInfo)			commandException(465, errorInfo + " :You are banned from this server")
#define ERR_CHANNELISFULL(errorInfo)			commandException(471, errorInfo + " :Cannot join channel (+l)")
#define ERR_UNKNOWNMODE(errorInfo)				commandException(472, errorInfo + " :is unknown mode char to me")
#define ERR_INVITEONLYCHAN(errorInfo)			commandException(473, errorInfo + " :Cannot join channel (+i)")
#define ERR_CHANOPRIVSNEEDED(errorInfo)			commandException(482, errorInfo + " :You're not channel operator")


typedef struct structured_message
{
	std::string				nickname;
	std::string				user;
	std::string				host;
	std::string				command;
	std::list<std::string>	params;
}							struct_msg;

bool		isspecial(char c);
void		format_nickname(std::string nick);
bool		format_channel(std::string chan);
bool		format_key(std::string key);
bool		msgto_nickname(std::string nick);
struct_msg	structuring_message(std::string message);
int			parseArgs(char **argv);

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
