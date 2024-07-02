/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvincent <lvincent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 13:02:19 by r                 #+#    #+#             */
/*   Updated: 2024/07/02 22:21:05 by lvincent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP
# include <iostream>
# include <string>

// Class definition
class Client
{
	private:
		int			_fd;
		std::string	_nickname;
		std::string	_username;
		std::string _realname;
 		std::string _messageBuffer;
		std::string _sendBuffer;
		bool		_pass;
		bool		_toDisconnect;
		bool		_isRegistered;
	public:
	// Constructor 
		Client(); 
		Client(const Client &rhs); 
		Client(int newFd);
		~Client(); 
		Client& operator=(const Client &rhs); 
	
	// Getters 
	int			getFd(void) const					{ return _fd; }
	std::string getNickname(void) const				{if (!_nickname.empty()) return _nickname; else return "*"; }
	std::string getUsername(void) const				{ return _username; }
	std::string	getRealname(void) const				{ return _realname; }
	std::string	getMessageBuffer(void) const		{ return _messageBuffer; }
	bool		getPass(void) const					{ return _pass; }
	std::string getSendBuffer(void) const			{ return _sendBuffer; }
	bool		getDisconnect(void) const			{ return _toDisconnect; }
	bool		getRegistered(void) const			{ return _isRegistered; }
	
	// Setters 
		void		setFd(int fd); 
		void		setNickname(std::string nickname); 
		void		setUsername(std::string username);
		void		setRealname(std::string realname);
		void		setMessageBuffer(std::string msg);
		void		appendMessageBuffer(std::string msg);
		void		setPass(bool state);
		void		setSendBuffer(std::string msg);
		void		appendSendBuffer(std::string msg);
		void		setDisconnect(bool state);
		void		setRegistered(bool state);
	
	// Output 
		void		output(void); 
}; 
 
#endif
