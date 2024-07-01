/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   userSettings.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvincent <lvincent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 14:27:35 by r                 #+#    #+#             */
/*   Updated: 2024/07/01 19:58:14 by lvincent         ###   ########.fr       */
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
	std::string client;
	if (!myClient.getNickname().empty())
		client = myClient.getNickname();
	else
		client = "*";
	if (msg.params.size() < 1)
		throw ERR_NONICKNAMEGIVEN(client);
	std::string nick = msg.params.front();
	format_nickname(client, nick);
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
	for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); it++)
	{
		if (it->second.getCl().find(fd) == it->second.getCl().end())
			continue ;
		messageToChannel(it->second.getCl(), QUIT_RPL(user_id(myClient.getNickname(), myClient.getUsername()), sendoff), fd);
		it->second.getCl().erase(fd);
		it->second.getOp().erase(fd);
		if (!it->second.getCl().size())
			this->_channels.erase(it++);
	}
	messageToClient(fd, QUIT_RPL(user_id(myClient.getNickname(), myClient.getUsername()), sendoff));
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

void	Server::command_name(struct_msg msg, int fd)
{
	Client&	myClient = this->getClient(fd);

	if (!myClient.getPass() || myClient.getNickname().empty() || myClient.getUsername().empty())
		throw ERR_NOTREGISTERED("*");

	if (msg.params.size())
	{
		std::stringstream	ss(*(msg.params.begin()));
		std::string 		chan;
		while (std::getline(ss, chan, ','))
		{
			it_chan it = this->_channels.find(chan);
			if (it == this->_channels.end())
				messageToClient(fd, RPL_ENDOFNAMES(myClient.getNickname(), chan));
			else
			{
				messageToClient(fd, RPL_NAMREPLY(chan, myClient.getNickname(), \
							clientList(it->second.getCl(), it->second.getOp())));
				messageToClient(fd, RPL_ENDOFNAMES(myClient.getNickname(), chan));
			}
		}
	}
	else
		for (it_chan it = this->_channels.begin(); it != this->_channels.end(); it++)
			messageToClient(fd, RPL_NAMREPLY(it->first, myClient.getNickname(), \
						clientList(it->second.getCl(), it->second.getOp())));
}
