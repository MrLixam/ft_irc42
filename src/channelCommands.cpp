/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channelCommands.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvincent <lvincent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 15:38:23 by lvincent          #+#    #+#             */
/*   Updated: 2024/06/17 18:19:41 by r                ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include <iostream>
#include <sstream>

void Server::command_join(struct_msg msg, int fd)
{
	Client	myClient = this->getClient(fd);

	if (!myClient.getPass() || myClient.getNickname().empty() || myClient.getUsername().empty())
		throw 451;
	if (msg.params.size() < 1)
        throw 431;
/*		if (msg.params.size() > 1)
		{
			if (!it->second.getPassword().empty() && !std::getline(pw, token2, ','))
				throw 464;
			if (it->second.getPassword() != token2)
				throw 464;
		}*/
}

void Server::command_privmsg(struct_msg msg, int fd)
{
	(void)msg;
	(void)fd;
}

void Server::command_part(struct_msg msg, int fd)
{
	Client	myClient = this->getClient(fd);

	if (!myClient.getPass() || myClient.getNickname().empty() || myClient.getUsername().empty())
		throw 451;
	if (msg.params.size() < 1)
        throw 431;
	std::list<std::string>::iterator	ms = msg.params.begin(); 
	std::stringstream	ss(*ms);
	std::string 		token;
	if (msg.params.size() > 1)
	{
		ms++;
		std::stringstream	pw(*ms);
		std::string			token2;
	}
	std::map<std::string, Channel>::iterator it;
	while (std::getline(ss, token, ','))
	{
		it = this->_channels.find(token);
		if (it == this->_channels.end())
			throw 403;
		if (it->second.getCl().find(fd) == it->second.getCl().end())
			throw 442;
//		if (msg.params.size() > 1)
			//send part and message
//		else
			//send part only
		it->second.getCl().erase(fd);
        it->second.getOp().erase(fd);
	}
}
