/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvincent <lvincent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 16:23:28 by r                 #+#    #+#             */
/*   Updated: 2024/06/14 14:19:47 by lvincent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCSERV_HPP
#define IRCSERV_HPP

#include <list>
#include <string>

#define	MAX_CLIENTS				1024

#define	ERR_NOSUCHNICK			401
#define	ERR_NOSUCHCHANNEL		403
#define	ERR_CANNOTSENDTOCHAN	404
#define	ERR_TOOMANYTARGETS		407
#define	ERR_NOTEXTTOSEND 		412
#define	ERR_UNKNOWNCOMMAND		421
#define	ERR_NONICKNAMEGIVEN		431
#define	ERR_ERRONEUSNICKNAME	432
#define	ERR_NICKNAMEINUSE		433
#define	ERR_USERNOTINCHANNEL	441
#define	ERR_NOTONCHANNEL		442
#define	ERR_USERONCHANNEL		443
#define	ERR_NOTREGISTERED		451
#define	ERR_NEEDMOREPARAMS		461
#define	ERR_ALREADYREGISTRED	462
#define	ERR_PASSWDMISMATCH		464
#define	ERR_YOUREBANNEDCREEP	465
#define	ERR_YOUWILLBEBANNED		466

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
struct_msg	structuring_message(std::string message);
int			parseArgs(char **argv);

#endif
