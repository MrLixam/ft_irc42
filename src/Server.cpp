/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvincent <lvincent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 17:27:40 by lvincent          #+#    #+#             */
/*   Updated: 2024/06/13 10:46:00 by lvincent         ###   ########.fr       */
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

	_servSocketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_servSocketFd == -1)
		throw std::runtime_error("init: socket creation failed: " + std::string(std::strerror(errno)));
	
	int opt = 1;
	if (setsockopt(_servSocketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) //make port reusable in case of restart etc.
	{
		close(_servSocketFd);
		throw std::runtime_error("init: setting socket option SO_REUSEADDR failed: " + std::string(std::strerror(errno)));
	}
	if (fcntl(_servSocketFd, F_SETFL, O_NONBLOCK) == -1) //make actions on port nonblocking
	{
		close(_servSocketFd);
		throw std::runtime_error("init: setting socket option O_NONBLOCK failed: " + std::string(std::strerror(errno)));
	}
	
	memset(&addr, 0, sizeof(addr));
	
	addr.sin_family = AF_INET; //IPV4 setup
	addr.sin_port = htons(_port); //int to actual port byte data conversion
	addr.sin_addr.s_addr = INADDR_ANY; //accept connexion from any ip adress
	if (addr.sin_port == 0) //just in case, should not really happen, but port 0 isn't valid
	{
		close(_servSocketFd);
		throw(std::runtime_error("init: invalid port number"));
	}
	
	if (bind(_servSocketFd, (sockaddr *)&addr, sizeof(addr)) == -1) //bind the socket to an interface
	{
		close(_servSocketFd);
		throw std::runtime_error("init: setting bind() on server socket failed: " + std::string(std::strerror(errno)));
	}
	if (listen(_servSocketFd, SOMAXCONN) == -1) //set backup queue of the server to max value
	{
		close(_servSocketFd);
		throw std::runtime_error("init: setting listen() on server socket failed: " + std::string(std::strerror(errno)));
	}
	
	ServPoll.fd = _servSocketFd;
	ServPoll.revents = 0;
	ServPoll.events = POLLIN; //setup server awaited events to read in

	_fdvec.push_back(ServPoll); //add fd to fd list for poll

	std::cout << "Server socket fd: " << _servSocketFd << std::endl;
	std::cout << "Server Started." << std::endl;
	std::cout << "Server port is: " << _port << std::endl;
}

int Server::newClient(std::vector<struct pollfd>& new_fd)
{
	struct sockaddr_in clientaddr;
	socklen_t size = sizeof(struct sockaddr_in);

	int client_sock = accept(_servSocketFd, (struct sockaddr *)&clientaddr, &size);
	if (client_sock == -1)
	{
		std::cout << "newClient(): accept() failed" << std::endl;
		return (-1);
	}
	std::cout << "newClient: " << client_sock << std::endl;
	struct pollfd client_pollfd;

	client_pollfd.revents = 0;
	client_pollfd.fd = client_sock;
	client_pollfd.events = POLLIN | POLLOUT; 

	new_fd.push_back(client_pollfd);
	_clients.insert(std::pair<int, Client>(client_sock, Client(client_sock, "default", "default")));
	std::cout << "Client id: " << client_sock << " connected" << std::endl;
	return (0);
}

std::map<std::string, int> initCmdMap(void)
{
	std::map<std::string, int> newMap;

	newMap["PASS"] = 0;
	newMap["NICK"] = 1;
	newMap["USER"] = 2;
	newMap["JOIN"] = 3;
	newMap["PRIVMSG"] = 4;
	newMap["KICK"] = 5;
	newMap["INVITE"] = 6;
	newMap["TOPIC"] = 7;
	newMap["MODE"] = 8;

	return (newMap);
}

void	Server::commands(std::string message)
{

	static std::map<std::string, int> CommandMap = initCmdMap();
	struct_msg	msg = structuring_message(message);

	std::map<std::string, int>::iterator it = CommandMap.find(msg.command);

	int commandSwitch = (it != CommandMap.end()) ? it->second : -1;
	try
	{
		if (msg.command.empty())
			throw 421;
		switch (commandSwitch)
		{
			case 0:
				// PASS command
				break;
			case 1:
				//NICK command
				break;
			case 2:
				//USER command
				break;
			case 3:
				//JOIN command
				break;
			case 4:
				//PRIVMSG command
				break;
			case 5:
				//KICK command
				break;
			case 6:
				//INVITE command
				break;
			case 7:
				//TOPIC command
				break;
			case 8:
				//MODE command
				break;
			default:
				throw 421;
		}

	}
	catch (int e)
	{
		std::cout << "send error code " << e << std::endl;
	}
}

void Server::receiveData(std::vector<struct pollfd>::iterator &it)
{
	char buffer[1024];
	std::string message;

	errno = 0;
	memset(buffer, 0, 1024);
	std::cout << "file descriptor:" << it->fd << " being read from" << std::endl;
	ssize_t rdBytes = recv(it->fd, buffer, 1024, 0);
	if (rdBytes == -1)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return;
		throw std::runtime_error("receiveData: recv() failed");
	}
	if (rdBytes == 0)
	{
		std::cout << "Client id: " << it->fd << " disconnected" << std::endl;
		_clients.erase(it->fd);
		close(it->fd);
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
		std::vector<struct pollfd> new_fds;
		for(std::vector<pollfd>::iterator it = _fdvec.begin(); it != _fdvec.end(); it++)
		{
			if (it->revents & POLLIN)
			{
				if (it->fd == _servSocketFd)
					newClient(new_fds);
				else
				{
					receiveData(it);
					if (it == _fdvec.end())
						break;
				}
			}
		}
		_fdvec.insert(_fdvec.end(), new_fds.begin(), new_fds.end());
	}
	while(_fdvec.size() != 0)
	{
		close(_fdvec[0].fd);
		_fdvec.erase(_fdvec.begin());
	}
	//deco du serv, il faudra fermer les fd, etc..
}
