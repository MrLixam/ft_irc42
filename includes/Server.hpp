/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvincent <lvincent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 15:52:26 by lvincent          #+#    #+#             */
/*   Updated: 2024/06/23 20:23:37 by lvincent         ###   ########.fr       */
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
#include "ircserv.hpp"
#include <vector>

extern bool server_signal;

typedef void (*cmdPtr)();

class Server
{
	private:
		std::map<int, Client>			_clients; //list of connected clients
		std::map<std::string, Channel>	_channels;
		std::vector<struct pollfd>		_fdvec;

		int								_port;	
		std::string						_password;	//arguments given as input.

		struct addrinfo					_hints;
		struct addrinfo					_servRes;
		int								_servSocketFd;
		size_t							_maxClients;
			
	public:		
		//Constructors
		Server(void);
		Server(int port, std::string password);
		~Server(void);

		//Getters
		int			getFd(void) const;
		int			getPort(void) const;
		std::string	getPwd(void) const;
		Client		getClient(int fd);
		
		//Setters
		void		setMaxClients(size_t i);

		//iterator
		typedef typename std::map<std::string, Client>::iterator it_client;
		typedef typename std::map<std::string, Channel>::iterator it_chan;

		//Member functions
		void		init(void);
		void		run(void);
		void		newClient(std::vector<struct pollfd>& new_fd);
		void		receiveData(std::vector<struct pollfd>::iterator &it);
		void		messageToChannel(std::set<int> fdList, std::string message);
		void		messageToChannel(std::set<int> fdList, std::string message, int senderFd);
		void		sendData(std::vector<struct pollfd>::iterator it);
		void		commands(std::string message, int fd);

		//Command function
		
			//userSettings.cpp
		void		command_pass(struct_msg msg, int fd);
		void		command_nick(struct_msg msg, int fd);
		void		command_user(struct_msg msg, int fd);
		void		command_quit(struct_msg msg, int fd);
		
			//channelCommands.cpp
		void		command_join(struct_msg msg, int fd);
		void		command_privmsg(struct_msg msg, int fd);
		void		command_part(struct_msg msg, int fd);
		void    	leave_chan(std::string chan, int fd, std::string msg);
		void    	join_chan(std::string chan, int fd, std::string key);
		
			//operatorCommands.cpp
		void		command_kick(struct_msg msg, int fd);
		void		command_invite(struct_msg msg, int fd);
		void		command_topic(struct_msg msg, int fd);
		void		command_mode(struct_msg msg, int fd);
		void		command_oper(struct_msg msg, int fd);

		//Utils
		bool		usernameExists(const std::string username, int fd) const;
};
#endif
