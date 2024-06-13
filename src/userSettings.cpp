/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   userSettings.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: r <marvin@42.fr>                           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 14:27:35 by r                 #+#    #+#             */
/*   Updated: 2024/06/13 15:54:45 by r                ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ircserv.hpp"
#include "../includes/Server.hpp"

void	Server::command_pass(struct_msg msg, int fd)
{
	Client	myClient = this->getClient(fd);

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
	Client	myClient = this->getClient(fd);

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
	(void)msg;
	(void)fd;
}
