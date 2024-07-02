/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operatorCommands.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvincent <lvincent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 15:46:00 by lvincent          #+#    #+#             */
/*   Updated: 2024/07/02 19:34:38 by gpouzet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include "../includes/replies.hpp"

void	Server::kick_user(std::string user_id, std::string nick, it_chan it, std::string user, std::string comment = "")
{
		int dest = usernameExists(user, -1);
		if (dest < 0)
			throw ERR_NOSUCHNICK(nick + " " + user);
		if (it->second.getCl().find(dest) == it->second.getCl().end())
			throw ERR_USERNOTINCHANNEL(nick + " " + user + " " + it->first);
		if (comment.empty())
			messageToChannel(it->second.getCl(), KICK_RPL(user_id, it->first, user, "Kicked by operator"));
		else
			messageToChannel(it->second.getCl(), KICK_RPL(user_id, it->first, user, comment));
		it->second.getCl().erase(dest);
		it->second.getOp().erase(dest);
}

void	Server::kick_users(std::string user_id, std::string nick, it_chan it, std::string users, std::string comment = "")
{
	std::stringstream	ss(users);
	std::string 		token;
	while (std::getline(ss, token, ','))
	{
		try
		{
			kick_user(user_id, nick, it, token, comment);
		}
		catch (commandException& e)
		{
			std::cout << "send error code " << e._errorCode << std::endl;
		}
	}
}

void	Server::command_kick(struct_msg msg, int fd)
{
	Client&			myClient = this->getClient(fd);

	if (!myClient.getPass() || myClient.getNickname().empty() || myClient.getUsername().empty())
		throw ERR_NOTREGISTERED("*");
	if (msg.params.size() < 2)
        throw ERR_NEEDMOREPARAMS(myClient.getNickname());
	std::list<std::string>::iterator	ms = msg.params.begin(); 
	bool solo_chan = (*ms).find(',') == std::string::npos;
	std::stringstream	chan(*ms);
	std::string 		token;
	std::string 		users = (*(++ms)).substr();
	std::stringstream	userstream(users);
	std::string			user;
	std::string 		comment = "";
	if (msg.params.size() > 2)
		comment = (*(++ms)).substr();
	while (std::getline(chan, token, ','))
	{
		try
		{
			it_chan it = this->_channels.find(token);
			if (it == this->_channels.end())
				throw ERR_NOSUCHCHANNEL(myClient.getNickname() + " " + token);
			if (it->second.getCl().find(fd) == it->second.getCl().end())
				throw ERR_NOTONCHANNEL(myClient.getNickname() + " " + it->first);
			if (it->second.getOp().find(fd) == it->second.getOp().end())
				throw ERR_CHANOPRIVSNEEDED(myClient.getNickname() + " " + it->first);
			if (solo_chan)
				kick_users(user_id(myClient.getNickname(), myClient.getUsername()), myClient.getNickname(), it, users, comment);
			else
			{
				if (!std::getline(userstream, user, ','))
					throw ERR_NEEDMOREPARAMS(myClient.getNickname() + " KICK");
				kick_user(user_id(myClient.getNickname(), myClient.getUsername()), myClient.getNickname(), it, user, comment);
			}
		}
		catch (commandException& e)
		{
			std::cout << "send error code " << e._errorCode << std::endl;
		}
	}
}

void	Server::command_invite(struct_msg msg, int fd)
{
	Client&	myClient = this->getClient(fd);

	if (!myClient.getPass() || myClient.getNickname().empty() || myClient.getUsername().empty())
		throw ERR_NOTREGISTERED("*");
	if (msg.params.size() < 2)
        throw ERR_NEEDMOREPARAMS(myClient.getNickname());
	std::list<std::string>::iterator	ms = msg.params.begin(); 
	std::string	nick = (*ms).substr();
	std::string	chan = (*(++ms)).substr();
	int dest = usernameExists(nick, -1);
	if (dest < 0)
		throw ERR_NOSUCHNICK(myClient.getNickname() + " " + nick);
	it_chan it = this->_channels.find(chan);
	if (it == this->_channels.end())
		throw ERR_NOSUCHCHANNEL(myClient.getNickname() + " " + it->first);
	if (it->second.getCl().find(fd) == it->second.getCl().end())
		throw ERR_NOTONCHANNEL(myClient.getNickname() + " " + it->first);
	if (it->second.getInvite() && it->second.getOp().find(fd) == it->second.getOp().end())
		throw ERR_CHANOPRIVSNEEDED(myClient.getNickname() + " " + it->first);
	if (it->second.getCl().find(dest) != it->second.getCl().end())
		throw ERR_USERONCHANNEL(myClient.getNickname() + " " + nick + " " + it->first);
	it->second.getInvited().insert(dest);
	messageToClient(fd, RPL_INVITING(myClient.getNickname(), this->getClient(dest).getNickname(), it->first));
	messageToClient(dest, msg_source(msg) + " INVITE " + this->getClient(dest).getNickname() + " " + chan + "\r\n");
}

void	Server::modes_switch(std::string nick, it_chan it, std::string modes, std::string param = "")
{
	if (modes[0] != '+' && modes[0] != '-')
		throw ERR_UNKNOWNMODE(nick + " " + modes[0]);
	for (int i = 1; modes[i]; i++)
	{
		try
		{
			if (modes[i] == 'i')
				it->second.setInvite(modes[0] == '+');
			else if (modes[i] == 't')
				it->second.setTopic_op(modes[0] == '+');
			else if (modes[i] == 'k')
			{
				if (param.empty())
					throw ERR_INVALIDMODEPARAM(nick + " " + it->first + " k " + param + " :invalid password");
				else if (modes[0] == '-' && it->second.getPassword() == param)
					it->second.setPassword(0);
				else (it->second.getPassword().empty())
					it->second.setPassword(param);
				else
					throw ERR_INVALIDMODEPARAM(nick + " " + it->first + " k " + param + " :Channel key already set");
			}
			else if (modes[i] == 'o')
			{
				if (param.empty())
					throw ERR_NEEDMOREPARAMS(nick);
				int user = usernameExists(param, -1);
				if (user < 0)
					throw ERR_NEEDMOREPARAMS(nick);
				if (it->second.getCl().find(user) == it->second.getCl().end())
					throw ERR_USERNOTINCHANNEL(nick + " " + param + " " + it->first);
				else if (modes[0] == '-')
					it->second.getOp().erase(user);
				else
					it->second.getOp().insert(user);
			}
			else if (modes[i] == 'l')
			{
				if (modes[0] == '-')
					it->second.setLimit(0);
				else if (param.empty() || param.find_first_not_of("0123456789") != std::string::npos)
					throw ERR_INVALIDMODEPARAM(nick + " " + it->first + " l " + param + " :invalid limit");
				else
					it->second.setLimit(atoi(param.c_str()));
			}
			else
				throw ERR_UNKNOWNMODE(nick + " " + modes[i]);
		}
		catch (commandException& e)
	}
}

void	Server::mode_reply(chan_modes save, it_chan it, std::string nick)
{
	if (it->second.getInvite() != save.invite)
	{
		if (it->second.getInvite())
			messageToChannel(clientList(it->second.getCl(), it->second.getOp()), RPL_CHANNELMODEIS(nick, it->first, "+i"));
		else
			messageToChannel(clientList(it->second.getCl(), it->second.getOp()), RPL_CHANNELMODEIS(nick, it->first, "-i"));
	}
	if (it->second.getTopic_op() != save.topic_op)
	{
		if (it->second.getTopic_op())
			messageToChannel(clientList(it->second.getCl(), it->second.getOp()), RPL_CHANNELMODEIS(nick, it->first, "+t"));
		else
			messageToChannel(clientList(it->second.getCl(), it->second.getOp()), RPL_CHANNELMODEIS(nick, it->first, "-t"));
	}
	if (it->second.getPassword() != save.password)
	{
		if (it->second.getPassword().empty())
			messageToChannel(clientList(it->second.getCl(), it->second.getOp()), RPL_CHANNELMODEIS(nick, it->first, "-k"));
		else
			messageToChannel(clientList(it->second.getCl(), it->second.getOp()), RPL_CHANNELMODEIS(nick, it->first, "+k " + it->second.getPassword()));
	}
	//need to do operator
	if (it->second.getLimit() != save.limit)
	{
		if (it->second.getLimit() == 0)
			messageToChannel(clientList(it->second.getCl(), it->second.getOp()), RPL_CHANNELMODEIS(nick, it->first, "-l"));
		else
		{
			std::stringstream ss;
    	    ss << this->_limit;
			messageToChannel(clientList(it->second.getCl(), it->second.getOp()), RPL_CHANNELMODEIS(nick, it->first, "+l " + ss.str()));
		}
	}
}

void	Server::command_mode(struct_msg msg, int fd)
{
	Client&	myClient = this->getClient(fd);

	if (!myClient.getPass() || myClient.getNickname().empty() || myClient.getUsername().empty())
		throw ERR_NOTREGISTERED("*");
	if (msg.params.size() < 1)
        throw ERR_NEEDMOREPARAMS(myClient.getNickname());
	std::list<std::string>::iterator	ms = msg.params.begin(); 
	std::string	chan = (*ms).substr();
	it_chan it = this->_channels.find(chan);
	if (it == this->_channels.end())
		throw ERR_NOSUCHCHANNEL(myClient.getNickname() + " " + chan);
	if (it->second.getCl().find(fd) == it->second.getCl().end())
		throw ERR_NOTONCHANNEL(myClient.getNickname() + " " + it->first);
	if (it->second.getOp().find(fd) == it->second.getOp().end())
		throw ERR_CHANOPRIVSNEEDED(myClient.getNickname() + " " + it->first);
	if (msg.params.size() == 1)
	{
		messageToClient(fd, RPL_CHANNELMODEIS(myClient.getNickname(), it->first, it->second.getModes()));
		return ;
	}
	//std::string	modes;
	std::vector<std::string>	modes;
	std::vector<std::string>	param;
	chan_modes save = it->second.getSaveModes();
	while (++ms != msg.params.end())
	{
		if ((*ms)[0] == '+' || (*ms)[0] == '-')
			modes.insert(*ms);
		else
			param.insert(*ms);
				/*
		std::list<std::string>::iterator	nextms = ms;
		nextms++;
		if (nextms != msg.params.end() && (*nextms)[0] != '-' && (*nextms)[0] != '+')
			modes_switch(myClient.getNickname(), it, modes, *(++ms));
		else
			modes_switch(myClient.getNickname(), it, modes);
			*/
	}
	std::vector<std::string>::iterator	mode_vec = parseMode(modes).begin();
	while (mode_vec != modes.end())
	{
		modes_switch(myClient.getNickname(), it, *mode_vec, param);
	}
	messageToChannel(clientList(it->second.getCl(), it->second.getOp()), RPL_CHANNELMODEIS(myClient.getNickname(), it->first, it->second.getModes()));
}
