/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: r <marvin@42.fr>                           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 13:28:08 by r                 #+#    #+#             */
/*   Updated: 2024/06/04 17:11:33 by r                ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP
# include <iostream>
# include <string>

// Class definition
class Channel
{
	private:
		std::set<Client>	_clients;
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
		std::string getTopic(); 
		bool 		getInvite(); 
		bool 		getTopic_op(); 
		std::string getPassword(); 
	// Setters 
		void setTopic(std::string _topic); 
		void setInvite(bool _invite); 
		void setTopic_op(bool _topic_op); 
		void setPassword(std::string _password); 
	// Output 
		void output(); 
}; 
 
#endif
