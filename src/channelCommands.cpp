/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channelCommands.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvincent <lvincent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 15:38:23 by lvincent          #+#    #+#             */
/*   Updated: 2024/06/25 00:06:42 by lvincent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include <iostream>
#include <sstream>

void	Server::leave_chan(std::string chan, int fd, std::string msg = "")
{
	it_chan it;
	it = this->_channels.find(chan);
	if (it == this->_channels.end())
		throw 403;
	if (it->second.getCl().find(fd) == it->second.getCl().end())
		throw 442;
	(void)msg;
	/*	if (msg.empty())
		//send part
	else
		//send part with msg
*/	it->second.getCl().erase(fd);
	it->second.getOp().erase(fd);
}

void	Server::join_chan(std::string chan, int fd, std::string key = "")
{
	it_chan it;
	it = this->_channels.find(chan);
	if (it == this->_channels.end())
		throw 403;
	if (it->second.getLimit() > 0 && it->second.getLimit() >= it->second.getCl().size())
		throw 471;
	if (it->second.getInvite())
		throw 473;
	if (it->second.getCl().find(fd) != it->second.getCl().end())
		throw "already on channel";
	if (!it->second.getPassword().empty() && key.empty())
		throw 464;
	if (!key.empty() && it->second.getPassword() != key)
		throw 464;
	it->second.getCl().insert(fd);
	//send 332    RPL_TOPIC and 353    RPL_NAMREPLY;
}

void	Server::command_join(struct_msg msg, int fd)
{
	Client&	myClient = this->getClient(fd);

	if (!myClient.getPass() || myClient.getNickname().empty() || myClient.getUsername().empty())
		throw 451;
	if (msg.params.size() < 1)
        throw 431;
	std::list<std::string>::iterator	ms = msg.params.begin(); 
	if (*ms == "0")
	{
		for (it_chan it = this->_channels.begin(); it != this->_channels.end(); it++)
			if (it->second.getCl().find(fd) != it->second.getCl().end())
				leave_chan(it->first, fd);
		return ;
	}
	std::stringstream	ss(*ms);
	std::string 		token;
	if (msg.params.size() > 1)
		ms++;
	std::stringstream	pw(*ms);
	std::string			key;
	while (std::getline(ss, token, ','))
	{
		try
		{
			if (msg.params.size() > 1 && std::getline(pw, key, ','))
				join_chan(token, fd, key);
			else
				join_chan(token, fd);
		}
		catch (int e)
		{
			std::cout << "send error code " << e << std::endl;
		}
	}
}

void	Server::command_privmsg(struct_msg msg, int fd)
{
	Client&	myClient = this->getClient(fd);

	if (!myClient.getPass() || myClient.getNickname().empty() || myClient.getUsername().empty())
		throw 451;
	if (msg.params.size() < 2)
        throw 431;
	std::list<std::string>::iterator	ms = msg.params.begin(); 
	std::stringstream	ss(*ms);
	std::string 		msgto;
	while (std::getline(ss, msgto, ','))
	{/*
		try
		{
			if (msgto_channel(msgto))
				return ;//send to channel
			else if (msgto_user(msgto))
				return ;//send to user
			else if (msgto_user(msgto))
				return ;//send to nickname
			else
				throw 411;
		}
		catch (int e)
		{
			std::cout << "send error code " << e << std::endl;
		}*/
	}
	//check nickname channel name and username
}

void	Server::command_part(struct_msg msg, int fd)
{
	Client&	myClient = this->getClient(fd);

	if (!myClient.getPass() || myClient.getNickname().empty() || myClient.getUsername().empty())
		throw 451;
	if (msg.params.size() < 1)
        throw 431;
	std::list<std::string>::iterator	ms = msg.params.begin(); 
	std::stringstream	ss(*ms);
	std::string 		token;
	while (std::getline(ss, token, ','))
	{
		try
		{
			if (msg.params.size() > 1)
				leave_chan(token, fd, *++ms);
			else
				leave_chan(token, fd);
		}
		catch (int e)
		{
			std::cout << "send error code " << e << std::endl;
		}
	}
}
