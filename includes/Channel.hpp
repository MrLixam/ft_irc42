/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvincent <lvincent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 13:28:08 by r                 #+#    #+#             */
/*   Updated: 2024/07/02 14:26:34 by r                ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP
# include <iostream>
# include <string>
# include <cstdlib>
# include <sstream>
# include <set>
# include "Client.hpp"

// Class definition
class Channel
{
	private:
		std::set<int>		_clients;
		std::set<int>		_operators;
		std::string			_topic;
		bool				_invite;
		std::set<int>		_invited;
		bool				_topic_op;
		std::string			_password;
		size_t				_limit;

	public:
	// Constructor 
		Channel(); 
		Channel(int	fd); 
		Channel(int	fd, std::string key); 
		Channel(const Channel &rhs);
		~Channel(); 
		Channel& operator=(const Channel &rhs); 

	// Getters 
		std::set<int>&	getCl(void);
		std::set<int>&	getOp(void);
		std::string		getTopic(void) const;
		bool 			getInvite(void) const;
		std::set<int>&	getInvited(void);
		bool 			getTopic_op(void) const;
		std::string		getPassword(void) const;
		size_t			getLimit(void) const;
		std::string		getModes(void) const;
	
	// Setters 
		void			setTopic(std::string topic); 
		void			setInvite(bool invite); 
		void			setTopic_op(bool topic_op); 
		void			setPassword(std::string password); 
		void			setLimit(int limit);
	
	// Output 
		void	output(void) const; 
}; 
 
#endif
