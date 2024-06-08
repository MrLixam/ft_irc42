/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvincent <lvincent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 13:28:06 by r                 #+#    #+#             */
/*   Updated: 2024/06/08 10:49:09 by lvincent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
// Constructor initializes attributes to 0 by default 

Channel::Channel() : _topic(0), _invite(0), _topic_op(0), _password(0)
{
	std::cout << "Channel constructor called" << std::endl;
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
	std::string Channel::getTopic(void) const		{ return _topic; }
	bool Channel::getInvite(void) const				{ return _invite; }
	bool Channel::getTopic_op(void) const			{ return _topic_op; }
	std::string Channel::getPassword(void) const	{ return _password; }
 
// Setters 
	void Channel::setTopic(std::string newTopic)		{ _topic = newTopic; }
	void Channel::setInvite(bool newInvite)				{ _invite = newInvite; }
	void Channel::setTopic_op(bool newTopic_op)			{ _topic_op = newTopic_op; }
	void Channel::setPassword(std::string newPassword)	{ _password = newPassword; }
 
// Output
	void Channel::output(void) const
{	std::cout << "clients : placeholder" << std::endl;
	std::cout << "topic : " << _topic << std::endl;
	std::cout << "invite : " << _invite << std::endl;
	std::cout << "topic_op : " << _topic_op << std::endl;
	std::cout << "password : " << _password << std::endl;
}
 
