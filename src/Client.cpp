/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvincent <lvincent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 13:03:14 by r                 #+#    #+#             */
/*   Updated: 2024/06/08 11:02:46 by lvincent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
// Constructor initializes attributes to 0 by default 

Client::Client() : _fd(0), _nickname(0), _username(0)
{
	std::cout << "Client constructor called" << std::endl;
}

Client::Client(const Client& rhs)
{
	std::cout << "Client copy constructor called" << std::endl;
	*this = rhs;
}

Client::Client(int newFd, std::string newNickname, std::string newUsername) : _fd(newFd), _nickname(newNickname), _username(newUsername)
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
	int Client::getFd(void) const				{ return _fd; }
	std::string Client::getNickname(void) const	{ return _nickname; }
	std::string Client::getUsername(void) const	{ return _username; }
 
// Setters 
	void Client::setFd(int fd)							{ _fd = fd; }
	void Client::setNickname(std::string nickname)		{ _nickname = nickname; }
	void Client::setUsername(std::string username)		{ _username = username; }
 
// Output
void Client::output(void)
{	std::cout << "fd : " << _fd << std::endl;
	std::cout << "nickname : " << _nickname << std::endl;
	std::cout << "username : " << _username << std::endl;
}
 
