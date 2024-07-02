/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvincent <lvincent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 13:28:06 by r                 #+#    #+#             */
/*   Updated: 2024/07/02 21:54:12 by gpouzet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Channel.hpp"
#include <sys/socket.h>
// Constructor initializes attributes to 0 by default 

Channel::Channel() : _invite(0), _topic_op(true), _password(""), _limit(0) {}

Channel::Channel(int fd) : _invite(0), _topic_op(true), _password(""), _limit(0)
{
	this->_clients.insert(fd);
	this->_operators.insert(fd);
}

Channel::Channel(int fd, std::string key) : _invite(0), _topic_op(true), _password(key), _limit(0)
{
	this->_clients.insert(fd);
	this->_operators.insert(fd);
}

Channel::Channel(const Channel& rhs)
{
	*this = rhs;
}

Channel::~Channel() {}

Channel	&Channel::operator=(const Channel& rhs)
{
	if (this != &rhs)
	{
		_clients = rhs._clients;
		_operators = rhs._operators;
		this->_topic = rhs._topic;
		this->_invite = rhs._invite;
		this->_invited = rhs._invited;
		this->_topic_op = rhs._topic_op;
		this->_password = rhs._password;
		this->_limit = rhs._limit;
	}
	return *this;
}

// Getters 
	std::set<int>&		Channel::getCl(void)						{ return _clients; }
	std::set<int>&		Channel::getOp(void)						{ return _operators; }
	std::string			Channel::getTopic(void) const				{ return _topic; }
	bool				Channel::getInvite(void) const				{ return _invite; }
	std::set<int>&		Channel::getInvited(void)					{ return _invited; }
	bool				Channel::getTopic_op(void) const			{ return _topic_op; }
	std::string			Channel::getPassword(void) const			{ return _password; }
	size_t				Channel::getLimit(void)	const				{ return _limit; }
	std::string			Channel::getModes(void) const
{
	std::string modes = "+";
	if (this->_invite)
		modes += "i";
	if (this->_topic_op)
		modes += "t";
	if (!this->_password.empty())
		modes += "k";
	if (this->_limit > 0)
		modes += "l";
	if (!this->_password.empty())
		modes += " " + this->_password;
	if (this->_limit > 0)
	{
		std::stringstream ss;
		ss << this->_limit;
		modes += " " + ss.str();
	}
	return (modes);
}
	
chan_modes	Channel::getSaveModes(void) const
{
	chan_modes	oldModes;

	oldModes.operators = _operators;
	oldModes.invite = _invite;
	oldModes.topic_op = _topic_op;
	oldModes.password = _password;
	oldModes.limit = _limit;
	return (oldModes);
}
 
// Setters 
	void				Channel::setTopic(std::string topic)		{ _topic = topic; }
	void				Channel::setInvite(bool invite)				{ _invite = invite; }
	void				Channel::setTopic_op(bool topic_op)			{ _topic_op = topic_op; }
	void				Channel::setPassword(std::string password)	{ _password = password; }
	void				Channel::setLimit(int limit)				{ _limit = limit; }
 
// Output
	void Channel::output(void) const
{	std::cout << "clients : placeholder" << std::endl;
	std::cout << "topic : " << _topic << std::endl;
	std::cout << "invite : " << _invite << std::endl;
	std::cout << "topic_op : " << _topic_op << std::endl;
	std::cout << "password : " << _password << std::endl;
}
