/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvincent <lvincent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 13:03:14 by r                 #+#    #+#             */
/*   Updated: 2024/06/25 00:04:03 by lvincent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"
// Constructor initializes attributes to 0 by default 

Client::Client() : _fd(0) {}

Client::Client(const Client& rhs)
{
	*this = rhs;
}

Client::Client(int newFd) : _fd(newFd), _pass(false) {}

Client::~Client() {}

Client	&Client::operator=(const Client& rhs)
{
	_fd = rhs._fd;
	_pass = false;
	return *this;
}

// Getters 
	int			Client::getFd(void) const					{ return _fd; }
	std::string Client::getNickname(void) const				{ return _nickname; }
	std::string Client::getUsername(void) const				{ return _username; }
	std::string	Client::getRealname(void) const				{ return _realname; }
	std::string	Client::getMessageBuffer(void) const		{ return _messageBuffer; }
	bool		Client::getPass(void) const					{ return _pass; }
	std::string Client::getSendBuffer(void) const			{ return _sendBuffer; }
	bool		Client::getDisconnect(void) const			{ return _toDisconnect; }
 
// Setters 
	void	Client::setFd(int fd)							{ _fd = fd; }
	void	Client::setNickname(std::string nickname)		{ _nickname = nickname; }
	void	Client::setUsername(std::string username)		{ _username = username; }
	void	Client::setRealname(std::string realname)		{ _realname = realname; }
	void	Client::setMessageBuffer(std::string msg)		{ _messageBuffer = msg; }
	void	Client::appendMessageBuffer(std::string msg)	{ _messageBuffer += msg; }
	void	Client::setPass(bool state)						{ _pass = state; }
	void	Client::setSendBuffer(std::string msg)			{ _sendBuffer = msg; }
	void	Client::appendSendBuffer(std::string msg)		{ _sendBuffer += msg; }
	void	Client::setDisconnect(bool state)				{ _toDisconnect = state; }
 
// Output
void Client::output(void)
{	std::cout << "fd : " << _fd << std::endl;
	std::cout << "nickname : " << _nickname << std::endl;
	std::cout << "username : " << _username << std::endl;
	std::cout << "pass : " << _pass << std::endl;
}
 
