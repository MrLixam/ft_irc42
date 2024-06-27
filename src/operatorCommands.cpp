/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operatorCommands.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvincent <lvincent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 15:46:00 by lvincent          #+#    #+#             */
/*   Updated: 2024/06/27 14:32:42 by r                ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

void	Server::kick_users(it_chan it, std::string users, std::string comment = "")
{
	std::stringstream	ss(users);
	std::string 		token;
	while (std::getline(ss, token, ','))
	{
		int dest = usernameExists(token, -1);
		if (dest < 0)
			throw 401;
		if (it->second.getCl().find(dest) == it->second.getCl().end())
			throw 441;
		it->second.getCl().erase(dest);
		it->second.getOp().erase(dest);
		if (comment.empty())
			messageToClient(dest, "Kicked from" + it->first);
		else
			messageToClient(dest, "Kicked from" + it->first + " :" + comment);
	}
}

void	Server::command_kick(struct_msg msg, int fd)
{
	Client&	myClient = this->getClient(fd);

	if (!myClient.getPass() || myClient.getNickname().empty() || myClient.getUsername().empty())
		throw 451;
	if (msg.params.size() < 2)
        throw 461;
	std::list<std::string>::iterator	ms = msg.params.begin(); 
	std::stringstream	chan(*ms);
	std::string 		token;
	ms++;
	std::string 		users = (*ms).substr();
	std::string 		comment = "";
	if (msg.params.size() > 2)
		comment = (*(++ms)).substr();
	while (std::getline(chan, token, ','))
	{
		try
		{
			it_chan it = this->_channels.find(token);
			if (it == this->_channels.end())
				throw 403;
			if (it->second.getCl().find(fd) == it->second.getCl().end())
				throw 442;
			if (it->second.getOp().find(fd) == it->second.getOp().end())
				throw 482;
			kick_users(it, users, comment);
		}
		catch (int e)
		{
			messageToClient(fd, getErrorMessage(*ms, e));
		}
	}
}

void	Server::command_invite(struct_msg msg, int fd)
{
	Client&	myClient = this->getClient(fd);

	if (!myClient.getPass() || myClient.getNickname().empty() || myClient.getUsername().empty())
		throw 451;
	if (msg.params.size() < 2)
        throw 461;
	std::list<std::string>::iterator	ms = msg.params.begin(); 
	std::string	nick = (*ms).substr();
	std::string	chan = (*(++ms)).substr();
	int dest = usernameExists(nick, -1);
	if (dest < 0)
		throw 401;
	it_chan it = this->_channels.find(chan);
	if (it == this->_channels.end())
	{
		join_chan(chan, fd, "");
		it = this->_channels.find(chan);
	}
	if (it->second.getCl().find(fd) == it->second.getCl().end())
		throw 441;
	if (it->second.getInvite() && it->second.getOp().find(fd) == it->second.getOp().end())
		throw 482;
	if (it->second.getCl().find(dest) != it->second.getCl().end())
		throw 443;
	it->second.getCl().insert(fd);
	//RPL_invinting
}

void	Server::modes_switch(it_chan it, std::string modes, std::string param = "")
{
	if (modes[0] != '+' && modes[0] != '-')
		throw 472;
	for (int i = 1; modes[i]; i++)
	{
		if (modes[i] == 'i')
			it->second.setInvite(modes[0] == '+');
		else if (modes[i] == 't')
			it->second.setTopic_op(modes[0] == '+');
		else if (modes[i] == 'k')
		{
			if (modes[0] == '-')
				it->second.setPassword(0);
			else if (param.empty() || !format_key(param))
				throw 461;
			else
				it->second.setPassword(param);
		}
		else if (modes[i] == 'o')
		{
			if (param.empty())
				throw 461;
			int user = usernameExists(param, -1);
			if (user < 0)
				throw 461;
			if (it->second.getCl().find(user) == it->second.getCl().end())
				throw 441;
			else if (modes[0] == '-')
				it->second.getOp().erase(user);
			else
				it->second.getOp().insert(user);
		}
		else if (modes[i] == 'l')
		{
			if (modes[0] == '-')
				it->second.setLimit(0);
			else if (param.empty() && param.find_first_not_of("0123456789") != std::string::npos)
				throw 461;
			else
				it->second.setLimit(atoi(param.c_str()));
		}
		else
			throw 472;
	}
}

void	Server::command_mode(struct_msg msg, int fd)
{
	Client&	myClient = this->getClient(fd);

	if (!myClient.getPass() || myClient.getNickname().empty() || myClient.getUsername().empty())
		throw 451;
	if (msg.params.size() < 2)
        throw 461;
	std::list<std::string>::iterator	ms = msg.params.begin(); 
	std::string	chan = (*ms).substr();
	it_chan it = this->_channels.find(chan);
	if (it == this->_channels.end())
		throw 403;
	if (it->second.getCl().find(fd) == it->second.getCl().end())
		throw 441;
	if (it->second.getOp().find(fd) == it->second.getOp().end())
		throw 482;
	std::string	modes;
	while (++ms != msg.params.end())
	{
		modes = (*ms).substr();
		std::list<std::string>::iterator	nextms = ms;
		nextms++;
		if (nextms != msg.params.end() && (*nextms)[0] != '-' && (*nextms)[0] != '+')
			modes_switch(it, modes, *++ms);
		else
			modes_switch(it, modes);
	}
}
