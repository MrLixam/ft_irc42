/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvincent <lvincent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 13:03:14 by r                 #+#    #+#             */
/*   Updated: 2024/06/13 13:20:43 by r                ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"
// Constructor initializes attributes to 0 by default 

Client::Client() : _fd(0)
{
	std::cout << "Client constructor called" << std::endl;
}

Client::Client(const Client& rhs)
{
	std::cout << "Client copy constructor called" << std::endl;
	*this = rhs;
}

Client::Client(int newFd) : _fd(newFd), _pass(false)
{

}

Client::~Client()
{
	std::cout << "Client destructor called" << std::endl;
}

Client	&Client::operator=(const Client& rhs)
{
	(void)rhs;
	return *this;
}

// Getters 
	int			Client::getFd(void) const			{ return _fd; }
	std::string Client::getNickname(void) const		{ return _nickname; }
	std::string Client::getUsername(void) const		{ return _username; }
	bool		Client::getPass(void) const			{ return _pass; }
 
// Setters 
	void	Client::setFd(int fd)						{ _fd = fd; }
	void	Client::setNickname(std::string nickname)	{ _nickname = nickname; }
	void	Client::setUsername(std::string username)	{ _username = username; }
	void	Client::setPass(bool state)					{ _pass = state; }
 
// Output
void Client::output(void)
{	std::cout << "fd : " << _fd << std::endl;
	std::cout << "nickname : " << _nickname << std::endl;
	std::cout << "username : " << _username << std::endl;
	std::cout << "pass : " << _pass << std::endl;
}
 
