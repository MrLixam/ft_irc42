/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvincent <lvincent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 13:02:19 by r                 #+#    #+#             */
/*   Updated: 2024/06/04 17:43:41 by lvincent         ###   ########.fr       */
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
 
	public:
	// Constructor 
		Client(); 
		Client( const Client &rhs); 
		Client(int newFd, std::string newNickname, std::string newUsername);
		~Client(); 
		Client& operator=( const Client &rhs); 
	// Getters 
		int getFd() const; 
		std::string getNickname() const; 
		std::string getUsername() const; 
	// Setters 
		void setFd(int _fd); 
		void setNickname(std::string _nickname); 
		void setUsername(std::string _username); 
	// Output 
		void output(); 
}; 
 
#endif
