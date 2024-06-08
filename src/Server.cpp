/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvincent <lvincent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 17:27:40 by lvincent          #+#    #+#             */
/*   Updated: 2024/06/08 14:04:21 by lvincent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ircserv.hpp"
#include "../includes/Server.hpp"
#include <arpa/inet.h>
#include <iostream>
#include <poll.h>
#include <sys/socket.h>
#include <stdexcept>
#include <fcntl.h>
#include <cstring>
#include <cstdio>
#include <cerrno>

Server::Server(void)
{
	_port = 4444;
	_password = "";
}

Server::Server(int port, std::string password)
{
	_port = port;
	_password = password;
	std::cout << "Server initialised !" << std::endl;
}

Server::~Server(void)
{
	std::cout << "Server Shutdown" << std::endl;
}

int			Server::getFd(void) const	{ return _servSocketFd; }
int			Server::getPort(void) const	{ return _port; }
std::string	Server::getPwd(void) const	{ return _password; }

void Server::init(void)
{
	std::cout << "Server Starting ..." << std::endl;
	
	pollfd ServPoll;
	sockaddr_in addr;

	addr.sin_family = AF_INET; //IPV4 setup
	addr.sin_port = htons(_port); //int to actual port byte data conversion
	addr.sin_addr.s_addr = INADDR_ANY; //accept connexion from any ip adress
	_servSocketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_servSocketFd == -1)
		throw std::runtime_error("init: socket creation failed");
	
	int opt = 1;
	if (setsockopt(_servSocketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) //make port reusable in case of restart etc.
		throw std::runtime_error("init: setting socket option SO_REUSEADDR failed");
	if (fcntl(_servSocketFd, F_SETFL, O_NONBLOCK) == -1) //make actions on port nonblocking
		throw std::runtime_error("init: setting socket option O_NONBLOCK failed");
	if (bind(_servSocketFd, (sockaddr *)&addr, sizeof(addr)) == -1) //bind the socket to an interface
		throw std::runtime_error("init: setting bind() on server socket failed");
	if (listen(_servSocketFd, SOMAXCONN) == -1) //set backup queue of the server to max value
		throw std::runtime_error("init: setting listen() on server socket failed");
	
	ServPoll.fd = _servSocketFd;
	ServPoll.revents = 0;
	ServPoll.events = POLLIN; //setup server awaited events to read in

	_fdvec.push_back(ServPoll); //add fd to fd list for poll

	std::cout << "Server Started." << std::endl;
}

int Server::newClient(void)
{
	sockaddr_in clientaddr;
	socklen_t size = sizeof(sockaddr_in);

	int client_sock = accept(_servSocketFd, (sockaddr *)&clientaddr, &size);
	if (client_sock == -1)
	{
		std::cout << "newClient(): accept() failed" << std::endl;
		return (-1);
	}
	std::cout << "newClient: " << client_sock << std::endl;
	pollfd client_pollfd;

	client_pollfd.fd = client_sock;
	client_pollfd.events = POLLIN | POLLOUT; 

	_fdvec.push_back(client_pollfd);
	_clients.insert(std::pair<int, Client>(client_sock, Client(client_sock, "default", "default")));
	std::cout << "Client id: " << _fdvec[1].fd << " connected" << std::endl;
	return (0);
}

void Server::receiveData(std::vector<pollfd>::iterator it)
{
	char buffer[1024];
	std::string message;

	errno = 0;
	std::cout << "file descriptor:" << it->fd << " being read from" << std::endl;
	ssize_t rdBytes = recv(it->fd, buffer, 1024, 0);
	if (rdBytes == -1)
	{
		perror("issue is: ");
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return;
		throw std::runtime_error("receiveData: recv() failed");
	}
	if (rdBytes == 0)
	{
		std::cout << "Client id: " << it->fd << " disconnected" << std::endl;
		_clients.erase(it->fd);
		it = _fdvec.erase(it);
		return;
	}
	message.append(buffer, rdBytes);
	std::cout << message << std::endl; 
}

void Server::run(void)
{
	while (server_signal == false)
	{
		if (poll(_fdvec.begin().base(), _fdvec.size(), -1) == -1)
		{
			if (server_signal == false)
				throw std::runtime_error("run: poll() failed");
			return ;
		}
		for (std::vector<pollfd>::iterator it = _fdvec.begin(); it != _fdvec.end(); it++)
		{
			if (it->revents & POLLIN)
			{
				if (it->fd == _servSocketFd)
					newClient();
				else
					receiveData(it);
			}
		}
	}
	//deco du serv, il faudra fermer les fd, etc..
}
