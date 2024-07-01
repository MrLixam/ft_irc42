/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvincent <lvincent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 13:28:06 by r                 #+#    #+#             */
/*   Updated: 2024/07/01 17:22:15 by r                ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Channel.hpp"
#include <sys/socket.h>
// Constructor initializes attributes to 0 by default 

Channel::Channel() : _invite(0), _topic_op(0), _limit(0)
{
	std::cout << "Channel constructor called" << std::endl;
}

Channel::Channel(int fd) : _invite(0), _topic_op(0), _limit(0)
{
	std::cout << "Channel constructor called" << std::endl;
	this->_clients.insert(fd);
	this->_operators.insert(fd);
}

Channel::Channel(int fd, std::string key) : _invite(0), _topic_op(0), _password(key), _limit(0)
{
	std::cout << "Channel constructor called" << std::endl;
	this->_clients.insert(fd);
	this->_operators.insert(fd);
}

Channel::Channel(const Channel& rhs)
{
	std::cout << "Channel copy constructor called" << std::endl;
	*this = rhs;
}

Channel::~Channel()
{
	std::cout << "Channel destructor called" << std::endl;
}

Channel	&Channel::operator=(const Channel& rhs)
{
	(void)rhs;
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
	std::string modes = "no modes";
	if (this->_invite == true)
		modes = "+i";
	if (this->_topic_op)
		modes + "t";
	if (!this->_password.empty())
		modes + "k";
	if (this->_limit > 0)
	{
		std::stringstream ss;
		ss << this->_limit;
		modes + "l " + ss.str();
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
