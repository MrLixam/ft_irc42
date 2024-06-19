/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvincent <lvincent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 13:28:08 by r                 #+#    #+#             */
/*   Updated: 2024/06/18 18:34:06 by lvincent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP
# include <iostream>
# include <string>
# include <map>
# include "Client.hpp"

// Class definition
class Channel
{
	private:
		std::map<int, Client>	_clients;
		std::map<int, Client>	_operators;
		std::string				_topic;
		bool					_invite;
		bool					_topic_op;
		std::string				_password;
		int						_limit;
	public:
	// Constructor 
		Channel(); 
		Channel(const Channel &rhs);
		~Channel(); 
		Channel& operator=(const Channel &rhs); 
	
	//Member functions
		void sendToAll(int senderFd, std::string message);
		void sendToAll(std::string message);

	// Getters 
		std::map<int, Client>	getCl(void) const;
		std::map<int, Client>	getOp(void) const;
		std::string				getTopic(void) const;
		bool 					getInvite(void) const;
		bool 					getTopic_op(void) const;
		std::string				getPassword(void) const;
		int						getLimit(void) const;
	
	// Setters 
		void					setTopic(std::string topic); 
		void					setInvite(bool invite); 
		void					setTopic_op(bool topic_op); 
		void					setPassword(std::string password); 
		void					setLimit(int limit);
	
	// Output 
		void	output(void) const; 
}; 
 
#endif
