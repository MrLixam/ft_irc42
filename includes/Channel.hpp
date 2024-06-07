/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvincent <lvincent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 13:28:08 by r                 #+#    #+#             */
/*   Updated: 2024/06/07 11:00:14 by lvincent         ###   ########.fr       */
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
		std::map<std::string, Client>	_clients;
		std::string			_topic;
		bool				_invite;
		bool				_topic_op;
		std::string			_password;
 
	public:
	// Constructor 
		Channel(); 
		Channel( const Channel &rhs); 
		~Channel(); 
		Channel& operator=( const Channel &rhs); 
	// Getters 
		std::string getTopic() const; 
		bool 		getInvite() const; 
		bool 		getTopic_op() const; 
		std::string getPassword() const; 
	// Setters 
		void setTopic(std::string _topic); 
		void setInvite(bool _invite); 
		void setTopic_op(bool _topic_op); 
		void setPassword(std::string _password); 
	// Output 
		void output() const; 
}; 
 
#endif
