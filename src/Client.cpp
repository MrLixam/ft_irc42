/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: r <marvin@42.fr>                           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 13:03:14 by r                 #+#    #+#             */
/*   Updated: 2024/06/04 13:55:51 by r                ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
// Constructor initializes attributes to 0 by default 

Client::Client() : _fd(0), _nickname(0), _username(0)
{
	std::cout << "Client constructor called" << std::endl;
}

Client::Client( const Client& rhs)
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

Client	&Client::operator=( const Client& rhs )
{
	(void)rhs;
	return *this;
}

// Getters 
	int Client::getFd() { return _fd; }
	std::string Client::getNickname() { return _nickname; }
	std::string Client::getUsername() { return _username; }
 
// Setters 
	void Client::setFd(int newFd) { _fd = newFd; }
	void Client::setNickname(std::string newNickname) { _nickname = newNickname; }
	void Client::setUsername(std::string newUsername) { _username = newUsername; }
 
// Output
	void Client::output()
{	std::cout << "fd : " << _fd << std::endl;
	std::cout << "nickname : " << _nickname << std::endl;
	std::cout << "username : " << _username << std::endl;
}
 
