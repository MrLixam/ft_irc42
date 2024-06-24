/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   userSettings.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvincent <lvincent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 14:27:35 by r                 #+#    #+#             */
/*   Updated: 2024/06/25 00:06:25 by lvincent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ircserv.hpp"
#include "../includes/Server.hpp"

void	Server::command_pass(struct_msg msg, int fd)
{
	Client&	myClient = this->getClient(fd);

	if (msg.params.size() != 1)
		throw 461;
	if (myClient.getPass())
		throw 462;
	if (msg.params.front() != this->getPwd())
		throw 464;

	myClient.setPass(true);
}

void	Server::command_nick(struct_msg msg, int fd)
{
	Client&	myClient = this->getClient(fd);

	if (!myClient.getPass())
		throw 451;
	if (msg.params.size() < 1)
		throw 431;
	std::string nick = msg.params.front();
	format_nickname(nick);
	if (usernameExists(nick, fd))
		throw 433;
	myClient.setNickname(nick);
}

void	Server::command_user(struct_msg msg, int fd)
{
	Client&	myClient = this->getClient(fd);

	if (!myClient.getPass())
		throw 451;
	if (!myClient.getUsername().empty())
		throw 462;
	if (msg.params.size() < 4)
		throw 431;
	std::list<std::string>::iterator it = msg.params.begin();
	myClient.setUsername(*it);
	std::advance(it, 3);
	myClient.setRealname(*it);
}

void	Server::command_quit(struct_msg msg, int fd)
{
	Client&	myClient = this->getClient(fd);

	if (!myClient.getPass() || myClient.getNickname().empty() || myClient.getUsername().empty())
		throw 451;
	(void)msg;
//	if (msg.params.size())
		//send message to all client of same channel + param 1
//	else
		//send message to all client of same channel
	for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		it->second.getCl().erase(fd);
		it->second.getOp().erase(fd);
	}
	getClient(fd).setDisconnect(true);
}
