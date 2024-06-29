/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvincent <lvincent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 16:23:28 by r                 #+#    #+#             */
/*   Updated: 2024/06/29 14:10:01 by lvincent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCSERV_HPP
#define IRCSERV_HPP

#include <iostream>
#include <list>
#include <string>

#define ERR(code, message)		commandException(code, message)

#define	ERR_NOSUCHNICK			ERR(401, "No such nick/channel")
#define	ERR_NOSUCHCHANNEL		ERR(403, "No such channel")
#define	ERR_CANNOTSENDTOCHAN	ERR(404, "Cannot send to channel")
#define	ERR_TOOMANYTARGETS		ERR(407, "recipients. Message not delivered")
#define ERR_NORECIPIENT			ERR(411, "No recipient given")
#define	ERR_NOTEXTTOSEND 		ERR(412, "No text to send")
#define	ERR_UNKNOWNCOMMAND		ERR(421, "Unknown command")
#define	ERR_NONICKNAMEGIVEN		ERR(431, "No nickname given")
#define	ERR_ERRONEUSNICKNAME	ERR(432, "Erroneous nickname")
#define	ERR_NICKNAMEINUSE		ERR(433, "Nickname is already in use")
#define	ERR_USERNOTINCHANNEL	ERR(441, "They aren't on that channel")
#define	ERR_NOTONCHANNEL		ERR(442, "You're not on that channel")
#define	ERR_USERONCHANNEL		ERR(443, "is already on channel")
#define	ERR_NOTREGISTERED		ERR(451, "You have not registered")
#define	ERR_NEEDMOREPARAMS		ERR(461, "Not enough parameters")
#define	ERR_ALREADYREGISTRED	ERR(462, "Unauthorized command (already registered)")
#define	ERR_PASSWDMISMATCH		ERR(464, "Password incorrect")
#define	ERR_YOUREBANNEDCREEP	ERR(465, "You are banned from this server")
#define	ERR_YOUWILLBEBANNED		ERR(466, "")
#define ERR_CHANNELISFULL		ERR(471, "Cannot join channel (+l)")
#define ERR_UNKNOWNMODE			ERR(472, "is unknown mode char to me")
#define ERR_INVITEONLYCHAN		ERR(473, "Cannot join channel (+i)")
#define ERR_CHANOPRIVSNEEDED	ERR(482, "You're not channel operator")

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
