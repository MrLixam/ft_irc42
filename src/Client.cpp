/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvincent <lvincent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 13:03:14 by r                 #+#    #+#             */
/*   Updated: 2024/06/30 21:07:49 by lvincent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"
// Constructor initializes attributes to 0 by default 

Client::Client() : _fd(0), _nickname(""), _username(""), _realname(""), _messageBuffer(""), _sendBuffer(""), _pass(false), _toDisconnect(false), _isRegistered(false) {}

Client::Client(const Client& rhs)
: _fd(rhs._fd), _nickname(rhs._nickname), _username(rhs._username), _realname(rhs._realname), _messageBuffer(rhs._messageBuffer), _sendBuffer(rhs._sendBuffer), _pass(rhs._pass), _toDisconnect(rhs._toDisconnect), _isRegistered(rhs._isRegistered) {}

Client::Client(int newFd) : _fd(newFd), _pass(false), _toDisconnect(false), _isRegistered(false){}

Client::~Client() {}

Client	&Client::operator=(const Client& rhs)
{
	_fd = rhs._fd;
	_pass = rhs._pass;
	_isRegistered = rhs._isRegistered;
	_toDisconnect = rhs._toDisconnect;
	_nickname = rhs._nickname;
	_username = rhs._username;
	_realname = rhs._realname;
	_messageBuffer = rhs._messageBuffer;
	_sendBuffer = rhs._sendBuffer;

	return *this;
}

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
	void	Client::setRegistered(bool state)				{ _isRegistered = state; }

// Output
void Client::output(void)
{	std::cout << "fd : " << _fd << std::endl;
	std::cout << "nickname : " << _nickname << std::endl;
	std::cout << "username : " << _username << std::endl;
	std::cout << "pass : " << _pass << std::endl;
}
 
