/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channelCommands.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvincent <lvincent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 15:38:23 by lvincent          #+#    #+#             */
/*   Updated: 2024/07/02 20:18:02 by lvincent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include "../includes/ircserv.hpp"
#include "../includes/colors.hpp"
#include "../includes/replies.hpp"

void	Server::leave_chan(std::string chan, int fd, std::string msg = "")
{
	it_chan it = this->_channels.find(chan);
	Client& tmp = getClient(fd);
	
	if (it == this->_channels.end())
		throw ERR_NOSUCHCHANNEL(tmp.getNickname() + " " + chan);
	if (it->second.getCl().find(fd) == it->second.getCl().end())
		throw ERR_NOTONCHANNEL(tmp.getNickname() + " " + chan);
	std::string userId = user_id(tmp.getNickname(), tmp.getUsername());
	if (msg.empty())
		messageToChannel(it->second.getCl(), PART_RPL_NOMSG(userId, it->first));
	else
		messageToChannel(it->second.getCl(), PART_RPL(userId, it->first, msg));
	it->second.getCl().erase(fd);
	it->second.getOp().erase(fd);
	if (!it->second.getCl().size())
		this->_channels.erase(chan);
}

void	Server::create_chan(std::string chan, int fd, std::string key = "")
{
	Client& tmp = getClient(fd);

	if (!format_channel(chan))
		throw ERR_NOSUCHCHANNEL(tmp.getNickname() + " " + chan);
	if (key.empty())
		_channels[chan] = Channel(fd);
	else
		_channels[chan] = Channel(fd, key);
}

void	Server::join_chan(std::string chan, int fd, std::string key = "")
{
	bool create = false;
	it_chan it = this->_channels.find(chan);
	Client& tmp = getClient(fd);
	
	if (it == this->_channels.end())
	{
		create_chan(chan, fd, key);
		it = this->_channels.find(chan);
		if (it == this->_channels.end())
			throw ERR_NOTREGISTERED("0");
		create = true;
	}
	if (it->second.getLimit() > 0 && it->second.getLimit() <= it->second.getCl().size())
		throw ERR_CHANNELISFULL(tmp.getNickname() + " " + it->first);
	if (it->second.getInvite() && it->second.getInvited().find(fd) == it->second.getInvited().end())
		throw ERR_INVITEONLYCHAN(tmp.getNickname() + " " + it->first);
	if (it->second.getCl().find(fd) != it->second.getCl().end() && !create)
		return ;
	if (!it->second.getPassword().empty() && key.empty())
		throw ERR_BADCHANNELKEY(tmp.getNickname() + " " + it->first);
	if (!key.empty() && it->second.getPassword() != key)
		throw ERR_PASSWDMISMATCH(tmp.getNickname());
	it->second.getCl().insert(fd);
	it->second.getInvited().erase(fd);
	
	messageToChannel(it->second.getCl(), JOIN_RPL(user_id(tmp.getNickname(), tmp.getUsername()), it->first));
	if (!it->second.getTopic().empty())
		messageToClient(fd, RPL_TOPIC(it->first, it->second.getTopic(), tmp.getNickname()));
	messageToClient(fd, RPL_NAMREPLY(it->first, tmp.getNickname(), clientList(it->second.getCl(), it->second.getOp())));
	messageToClient(fd, RPL_ENDOFNAMES(tmp.getNickname(), it->first));
}

void	Server::command_join(struct_msg msg, int fd)
{
	Client&	myClient = this->getClient(fd);

	if (!myClient.getPass() || myClient.getNickname().empty() || myClient.getUsername().empty())
		throw ERR_NOTREGISTERED("*");
	if (msg.params.size() < 1)
        throw ERR_NEEDMOREPARAMS(myClient.getNickname());
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
			std::cout << RED << "join: send error code " << e._errorCode << RESET << std::endl;
			std::ostringstream convert;
			convert << e._errorCode;
			std::string message = ":42IRC " + convert.str() + " " + e._errorMessage + "\r\n";
			myClient.appendSendBuffer(message);
		}
	}
}

void	Server::command_privmsg(struct_msg msg, int fd)
{
	Client&	myClient = this->getClient(fd);

	if (!myClient.getPass() || myClient.getNickname().empty() || myClient.getUsername().empty())
			throw ERR_NOTREGISTERED("*");
	if (msg.params.size() < 2)
        throw ERR_NOTEXTTOSEND(myClient.getNickname());
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
					throw ERR_NORECIPIENT(myClient.getNickname());
				messageToChannel(it->second.getCl(),":" + user_id(myClient.getNickname(), myClient.getUsername()) + " " + "PRIVMSG" + " " + msgto + " :" + *ms + "\r\n", fd);
			}
			else if (msgto_nickname(msgto))
			{
				int dest = usernameExists(msgto, -1);
				if (dest < 0)
					throw ERR_NOSUCHNICK(myClient.getNickname());
				messageToClient(dest, ":" + user_id(myClient.getNickname(), myClient.getUsername()) + " " + "PRIVMSG" + " " + msgto + " :" + *ms + "\r\n");
			}
			else
				throw ERR_NORECIPIENT(myClient.getNickname());
		}
		catch (commandException& e)
		{
			std::cout << RED << "privmsg: send error code " << e._errorCode << RESET << std::endl;
			std::ostringstream convert;
			convert << e._errorCode;
			std::string message = ":42IRC " + convert.str() + " " + e._errorMessage + "\r\n";
			myClient.appendSendBuffer(message);
		}
	}
}

void	Server::command_part(struct_msg msg, int fd)
{
	Client&	myClient = this->getClient(fd);

	if (!myClient.getPass() || myClient.getNickname().empty() || myClient.getUsername().empty())
		throw ERR_NOTREGISTERED("*");
	if (msg.params.size() < 1)
        throw ERR_NEEDMOREPARAMS(myClient.getNickname());
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
		catch (commandException& e)
		{
			std::cout << RED << "part: send error code " << e._errorCode << RESET << std::endl;
			std::ostringstream convert;
			convert << e._errorCode;
			std::string message = ":42IRC " + convert.str() + " " + e._errorMessage + "\r\n";
			myClient.appendSendBuffer(message);
		}
	}
}

void Server::command_topic(struct_msg msg, int fd)
{
	Client&	myClient = this->getClient(fd);

	if (!myClient.getPass() || myClient.getNickname().empty() || myClient.getUsername().empty())
		throw ERR_NOTREGISTERED("*");
	if (msg.params.size() < 1)
        throw ERR_NEEDMOREPARAMS(myClient.getNickname());
	std::list<std::string>::iterator	ms = msg.params.begin(); 
	it_chan it = this->_channels.find(*ms);
	if (it == this->_channels.end())
		throw ERR_NOTONCHANNEL(myClient.getNickname() + " " + *ms);
	if (msg.params.size() == 1)
	{
		if (it->second.getTopic().empty())
			messageToClient(fd, RPL_NOTOPIC(it->first, myClient.getNickname()));
		else
			messageToClient(fd, RPL_TOPIC(it->first, it->second.getTopic(), myClient.getNickname()));
	}
	if (msg.params.size() > 1)
	{
		if (it->second.getTopic_op() && it->second.getOp().find(fd) == it->second.getOp().end())
			throw ERR_CHANOPRIVSNEEDED(myClient.getNickname() + " " + it->first);
		it->second.setTopic(*++ms);
		std::set<int>& lst = it->second.getCl();
		for (std::set<int>::iterator it_lst = lst.begin(); it_lst != lst.end(); it_lst++)
			messageToClient(*it_lst, RPL_TOPIC(it->first, it->second.getTopic(), getClient(*it_lst).getNickname()));
	}
}
