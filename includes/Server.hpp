/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvincent <lvincent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 15:52:26 by lvincent          #+#    #+#             */
/*   Updated: 2024/06/11 12:03:14 by lvincent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <map>
#include <sys/socket.h>
#include <netdb.h>
#include <csignal>
#include <string>
#include <poll.h>
#include <sys/socket.h>
#include "Client.hpp"
#include "Channel.hpp"
#include <vector>

extern bool server_signal;

class Server
{
	private:
		std::map<int, Client>			_clients; //list of connected clients, std::set used for better lookup times
		std::map<std::string, Channel>	_channels;
		std::vector<struct pollfd>		_fdvec;

		int								_port;	
		std::string						_password;	//arguments given as input.

		struct addrinfo					_hints;
		struct addrinfo					_servRes;
		int								_servSocketFd;
			
	public:		
		//Constructors
		Server(void);
		Server(int port, std::string password);
		~Server(void);

		//Getters
		int			getFd(void) const;
		int			getPort(void) const;
		std::string	getPwd(void) const;
		
		//Member functions
		void		init(void);
		void		run(void);
		int			newClient(std::vector<struct pollfd>& new_fd);
		void		receiveData(std::vector<struct pollfd>::iterator &it);

		//Public member variables
};
#endif