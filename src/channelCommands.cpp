/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channelCommands.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvincent <lvincent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 15:38:23 by lvincent          #+#    #+#             */
/*   Updated: 2024/06/29 14:04:04 by lvincent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include "../includes/ircserv.hpp"
#include "../includes/replies.hpp"

void	Server::leave_chan(std::string chan, int fd, std::string msg = "")
{
	it_chan it = this->_channels.find(chan);
	if (it == this->_channels.end())
		throw ERR_NOSUCHCHANNEL;
	if (it->second.getCl().find(fd) == it->second.getCl().end())
		throw ERR_NOTONCHANNEL;
	(void)msg;
	Client& tmp = getClient(fd);
	std::string userId = user_id(tmp.getNickname(), tmp.getUsername());
	if (msg.empty())
		messageToChannel(it->second.getCl(), PART_RPL_NOMSG(userId, it->first));
	else
		messageToChannel(it->second.getCl(), PART_RPL(userId, it->first, msg));
	it->second.getCl().erase(fd);
	it->second.getOp().erase(fd);
}

void	Server::create_chan(std::string chan, int fd, std::string key = "")
{
	if (!format_channel(chan))
		throw ERR_NOSUCHCHANNEL;
	if (key.empty())
		_channels.insert(std::pair<std::string, Channel>(chan, Channel(fd)));
	else
	{
		if (!format_key(key))
			throw "bad key format";
		_channels.insert(std::pair<std::string, Channel>(chan, Channel(fd, key)));
	}
}

void	Server::join_chan(std::string chan, int fd, std::string key = "")
{
	it_chan it = this->_channels.find(chan);
	if (it == this->_channels.end())
	{
		create_chan(chan, fd, key);
		it = this->_channels.find(chan);
		it->second.getOp().insert(fd);
	}
	if (it->second.getLimit() > 0 && it->second.getLimit() >= it->second.getCl().size())
		throw ERR_CHANNELISFULL;
	if (it->second.getInvite())
		throw ERR_INVITEONLYCHAN;
	if (it->second.getCl().find(fd) != it->second.getCl().end())
		throw "already on channel";
	if (!it->second.getPassword().empty() && key.empty())
		throw ERR_PASSWDMISMATCH;
	if (!key.empty() && it->second.getPassword() != key)
		throw ERR_PASSWDMISMATCH;
	it->second.getCl().insert(fd);
	
	Client& tmp = getClient(fd);
	
	messageToClient(fd, JOIN_RPL(user_id(tmp.getNickname(), tmp.getUsername()), it->first));
	if (!it->second.getTopic().empty())
		messageToClient(fd, RPL_TOPIC(it->first, it->second.getTopic()));
	messageToClient(fd, RPL_NAMREPLY(it->first, tmp.getNickname(), clientList(it->second.getCl())));
	messageToClient(fd, RPL_ENDOFNAMES(it->first));
	
	//send a JOIN Message back to client as confirmation;
	//send 332    RPL_TOPIC and 353    RPL_NAMREPLY;
}

void	Server::command_join(struct_msg msg, int fd)
{
	Client&	myClient = this->getClient(fd);

	if (!myClient.getPass() || myClient.getNickname().empty() || myClient.getUsername().empty())
		throw ERR_NOTREGISTERED;
	if (msg.params.size() < 1)
        throw ERR_NEEDMOREPARAMS;
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
		catch (commandException& e)
		{
			std::cout << "send error code: " << e._errorCode << std::endl;
		}
	}
}

void	Server::command_privmsg(struct_msg msg, int fd)
{
	Client&	myClient = this->getClient(fd);

	if (!myClient.getPass() || myClient.getNickname().empty() || myClient.getUsername().empty())
		throw ERR_NOTREGISTERED;
	if (msg.params.size() < 2)
        throw 412;
	std::list<std::string>::iterator	ms = msg.params.begin(); 
	std::stringstream	ss(*ms);
	std::string 		msgto;
	ms++;
	while (std::getline(ss, msgto, ','))
	{
		try
		{
			if (format_channel(msgto))
			{
				it_chan it = this->_channels.find(msgto);
				if (it == this->_channels.end())
					throw 411;
				messageToChannel(it->second.getCl(), *ms);
			}
	//		else if (msgto_user(msgto))
	//			return ;//send to user
			else if (msgto_nickname(msgto))
			{
				int dest = usernameExists(msgto, -1);
				if (dest < 0)
					throw ERR_NOSUCHNICK;
				messageToClient(dest, *ms);
			}
			else
				throw ERR_NORECIPIENT;
		}
		catch (commandException& e)
		{
			std::cout << "send error code: " << e._errorCode << std::endl;
		}
	}
}

void	Server::command_part(struct_msg msg, int fd)
{
	Client&	myClient = this->getClient(fd);

	if (!myClient.getPass() || myClient.getNickname().empty() || myClient.getUsername().empty())
		throw ERR_NOTREGISTERED;
	if (msg.params.size() < 1)
        throw ERR_NEEDMOREPARAMS;
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

void Server::command_topic(struct_msg msg, int fd)
{
	Client&	myClient = this->getClient(fd);

	if (!myClient.getPass() || myClient.getNickname().empty() || myClient.getUsername().empty())
		throw ERR_NOTREGISTERED;
	if (msg.params.size() < 1)
        throw ERR_NEEDMOREPARAMS;
	std::list<std::string>::iterator	ms = msg.params.begin(); 
	it_chan it = this->_channels.find(*ms);
	if (it == this->_channels.end())
		throw ERR_NOTONCHANNEL;
	if (msg.params.size() == 1)
	{
		if (it->second.getTopic().empty())
			messageToClient(fd, *ms + " :No topic is set");
		else
			messageToClient(fd, *ms + " :" + it->second.getTopic());
	}
	if (msg.params.size() > 1)
	{
		if (it->second.getTopic_op() && it->second.getOp().find(fd) == it->second.getOp().end())
			throw ERR_CHANOPRIVSNEEDED;
		it->second.setTopic(*++ms);
	}
}
