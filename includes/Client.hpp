/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvincent <lvincent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 13:02:19 by r                 #+#    #+#             */
/*   Updated: 2024/06/25 00:23:19 by lvincent         ###   ########.fr       */
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
		int 		getFd(void) const; 
		std::string	getNickname(void) const; 
		std::string	getUsername(void) const;
		std::string	getRealname(void) const;
		std::string	getMessageBuffer(void) const;
		bool		getPass(void) const;
		std::string	getSendBuffer(void) const;
		bool		getDisconnect(void) const;
		bool		getRegistered(void) const;
	
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
