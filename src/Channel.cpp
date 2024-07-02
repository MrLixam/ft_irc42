/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvincent <lvincent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 13:28:06 by r                 #+#    #+#             */
/*   Updated: 2024/07/02 14:09:04 by lvincent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Channel.hpp"
#include <sys/socket.h>
// Constructor initializes attributes to 0 by default 

Channel::Channel() : _invite(0), _topic_op(0), _password(""), _limit(0) {}

Channel::Channel(int fd) : _invite(0), _topic_op(0), _password(""), _limit(0)
{
	this->_clients.insert(fd);
	this->_operators.insert(fd);
}

Channel::Channel(int fd, std::string key) : _invite(0), _topic_op(0), _password(key), _limit(0)
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
	bool				Channel::getTopic_op(void) const			{ return _topic_op; }
	std::string			Channel::getPassword(void) const			{ return _password; }
	size_t				Channel::getLimit(void)	const				{ return _limit; }
	std::string			Channel::getModes(void) const
{
	std::string modes = "no modes actif";
	if (this->_invite)
		modes = "+i";
	if (this->_topic_op)
	{
		if (modes == "no modes actif")
			modes = "+t";
		else
			modes += "t";
	}
	if (!this->_password.empty())
	{
		if (modes == "no modes actif")
			modes = "+k";
		else
			modes += "k";
	}
	if (this->_limit > 0)
	{
		std::stringstream ss;
		ss << this->_limit;
		if (modes == "no modes actif")
			modes = "+l " + ss.str();
		else
			modes += "l " + ss.str();
	}
	return (modes);
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
