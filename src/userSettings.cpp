/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   userSettings.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvincent <lvincent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 14:27:35 by r                 #+#    #+#             */
/*   Updated: 2024/06/30 18:59:44 by lvincent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ircserv.hpp"
#include "../includes/Server.hpp"
#include "../includes/replies.hpp"

void	Server::command_pass(struct_msg msg, int fd)
{
	Client&	myClient = this->getClient(fd);

	if (msg.params.size() != 1)
		throw ERR_NEEDMOREPARAMS("*");
	if (myClient.getPass())
		throw ERR_ALREADYREGISTRED(myClient.getNickname());
	if (msg.params.front() != _password)
		throw ERR_PASSWDMISMATCH("*");

	myClient.setPass(true);
}

void	Server::command_nick(struct_msg msg, int fd)
{
	Client&	myClient = this->getClient(fd);

	if (!myClient.getPass())
		throw ERR_NOTREGISTERED("*");
	if (msg.params.size() < 1)
	{
		if (myClient.getNickname().empty())
			throw ERR_NONICKNAMEGIVEN("*");
		else
			throw ERR_NONICKNAMEGIVEN(myClient.getNickname());
	}
	std::string nick = msg.params.front();
	format_nickname(nick);
	if (usernameExists(nick, fd) > 0)
	{
		if (myClient.getNickname().empty())
			throw ERR_NICKNAMEINUSE("* " + nick);
		else
			throw ERR_NICKNAMEINUSE(myClient.getNickname() + " " + nick);
	}
	myClient.setNickname(nick);
}

void	Server::command_user(struct_msg msg, int fd)
{
	Client&	myClient = this->getClient(fd);

	if (!myClient.getPass())
		throw ERR_NOTREGISTERED("*");
	if (!myClient.getUsername().empty())
		throw ERR_ALREADYREGISTRED(myClient.getNickname());
	if (msg.params.size() < 4)
	{
		if (myClient.getNickname().empty())
			throw ERR_NONICKNAMEGIVEN("*");
		else
			throw ERR_NONICKNAMEGIVEN(myClient.getNickname());
	}
	std::list<std::string>::iterator it = msg.params.begin();
	myClient.setUsername(*it);
	std::advance(it, 3);
	myClient.setRealname(*it);
}

void	Server::command_quit(struct_msg msg, int fd)
{
	Client&	myClient = this->getClient(fd);
	std::string	sendoff;

	if (!myClient.getPass() || myClient.getNickname().empty() || myClient.getUsername().empty())
		throw ERR_NOTREGISTERED("*");
	if (msg.params.size())
	{
		std::list<std::string>::iterator    ms = msg.params.begin();
		sendoff = myClient.getNickname() + " has quit:" + *ms;
	}
	else
		sendoff = myClient.getNickname() + " has quit.";
	for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		messageToChannel(it->second.getCl(), sendoff);
		it->second.getCl().erase(fd);
		it->second.getOp().erase(fd);
	}
	messageToClient(fd, QUIT_RPL(user_id(myClient.getNickname(), myClient.getUsername()), "Client Quit"));
	myClient.setDisconnect(true);
}

void	Server::command_ping(struct_msg msg, int fd)
{
	Client&	myClient = this->getClient(fd);

	if (!myClient.getPass() || myClient.getNickname().empty() || myClient.getUsername().empty())
		throw ERR_NOTREGISTERED("*");
	if (msg.params.size() != 1)
		throw ERR_NEEDMOREPARAMS(myClient.getNickname());
	messageToClient(fd, PONG_RPL(user_id(myClient.getNickname(), myClient.getUsername()), "42IRC"));

}