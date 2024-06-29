/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvincent <lvincent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 16:23:28 by r                 #+#    #+#             */
/*   Updated: 2024/06/29 13:56:06 by lvincent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCSERV_HPP
#define IRCSERV_HPP

#include <iostream>
#include <list>
#include <string>

#define	ERR_NOSUCHNICK			commandException(401, "No such nick/channel")
#define	ERR_NOSUCHCHANNEL		commandException(403, "No such channel")
#define	ERR_CANNOTSENDTOCHAN	commandException(404, "Cannot send to channel")
#define	ERR_TOOMANYTARGETS		commandException(407, "recipients. Message not delivered")
#define ERR_NORECIPIENT			commandException(411, "No recipient given")
#define	ERR_NOTEXTTOSEND 		commandException(412, "No text to send")
#define	ERR_UNKNOWNCOMMAND		commandException(421, "Unknown command")
#define	ERR_NONICKNAMEGIVEN		commandException(431, "No nickname given")
#define	ERR_ERRONEUSNICKNAME	commandException(432, "Erroneous nickname")
#define	ERR_NICKNAMEINUSE		commandException(433, "Nickname is already in use")
#define	ERR_USERNOTINCHANNEL	commandException(441, "They aren't on that channel")
#define	ERR_NOTONCHANNEL		commandException(442, "You're not on that channel")
#define	ERR_USERONCHANNEL		commandException(443, "is already on channel")
#define	ERR_NOTREGISTERED		commandException(451, "You have not registered")
#define	ERR_NEEDMOREPARAMS		commandException(461, "Not enough parameters")
#define	ERR_ALREADYREGISTRED	commandException(462, "Unauthorized command (already registered)")
#define	ERR_PASSWDMISMATCH		commandException(464, "Password incorrect")
#define	ERR_YOUREBANNEDCREEP	commandException(465, "You are banned from this server")
#define	ERR_YOUWILLBEBANNED		commandException(466, "")
#define ERR_CHANNELISFULL		commandException(471, "Cannot join channel (+l)")
#define ERR_UNKNOWNMODE			commandException(472, "is unknown mode char to me")
#define ERR_INVITEONLYCHAN		commandException(473, "Cannot join channel (+i)")
#define ERR_CHANOPRIVSNEEDED	commandException(482, "You're not channel operator")

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
