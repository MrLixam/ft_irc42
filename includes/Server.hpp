/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvincent <lvincent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 15:52:26 by lvincent          #+#    #+#             */
/*   Updated: 2024/07/01 16:20:27 by lvincent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "ircserv.hpp"
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
#include <iostream>
#include <sstream>
#include <ctime>
#include <cstdlib>

extern bool server_signal;

		typedef std::map<std::string, Channel>::iterator it_chan;

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
		std::string						_creationDate;
			
	public:		
		//Constructors
		Server(void);
		Server(int port, std::string password, struct tm * timeinfo);
		~Server(void);

		//Getters
		int			getFd(void) const;
		int			getPort(void) const;
		std::string	getPwd(void) const;
		Client&		getClient(int fd);

		
		//Setters
		void		setMaxClients(size_t i);

		//Member functions
		void		init(void);
		void		run(void);
		void		newClient(std::vector<struct pollfd>& new_fd);
		void		receiveData(struct pollfd& it, size_t i);
		void		messageToChannel(std::set<int> fdList, std::string message);
		void		messageToChannel(std::set<int> fdList, std::string message, int senderFd);
		void		sendData(struct pollfd& it, size_t i);
		void		commands(std::string message, int fd);
		void		messageToClient(int fd, std::string message);
		void		messageOfTheDay(std::string message, Client& client);

		//Command function
		
			//userSettings.cpp
		void		command_pass(struct_msg msg, int fd);
		void		command_nick(struct_msg msg, int fd);
		void		command_user(struct_msg msg, int fd);
		void		command_quit(struct_msg msg, int fd);
		void		command_ping(struct_msg msg, int fd);
		void		command_name(struct_msg msg, int fd);
		
			//channelCommands.cpp
		void		command_join(struct_msg msg, int fd);
		void		command_privmsg(struct_msg msg, int fd);
		void		command_part(struct_msg msg, int fd);
		void    	leave_chan(std::string chan, int fd, std::string msg);
		void		create_chan(std::string chan, int fd, std::string key);
		void    	join_chan(std::string chan, int fd, std::string key);
		void		command_topic(struct_msg msg, int fd);
		
			//operatorCommands.cpp
		void		kick_user(std::string user_id, std::string nick, it_chan it, std::string user, std::string comment);
		void		kick_users(std::string user_id, std::string nick, it_chan it, std::string users, std::string comment);
        void		command_kick(struct_msg msg, int fd);
        void		command_invite(struct_msg msg, int fd);
        void		modes_switch(std::string nick, it_chan it, std::string modes, std::string param);
        void		command_mode(struct_msg msg, int fd);

		//Utils
		int			usernameExists(const std::string username, int fd) const;
		std::string	clientList(std::set<int>& fdList, std::set<int> opList);
};
#endif
