/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvincent <lvincent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 17:27:40 by lvincent          #+#    #+#             */
/*   Updated: 2024/06/22 21:54:13 by lvincent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ircserv.hpp"
#include "../includes/Server.hpp"
#include "../includes/colors.hpp"
#include <arpa/inet.h>
#include <iostream>
#include <poll.h>
#include <sys/socket.h>
#include <sys/resource.h>
#include <stdexcept>
#include <fcntl.h>
#include <cstring>
#include <sstream>
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

int			Server::getFd(void) const		{ return _servSocketFd; }
int			Server::getPort(void) const		{ return _port; }
std::string	Server::getPwd(void) const		{ return _password; }

void		Server::setMaxClients(size_t i)	{ _maxClients = i; }

void Server::init(void)
{
	std::cout << YELLOW << "Server Starting ..." << RESET << std::endl;
	
	pollfd ServPoll;
	struct addrinfo hints, *servInfo, *tmp;
	int opt = 1;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = getprotobyname("TCP")->p_proto;
	hints.ai_flags = AI_PASSIVE;

	std::stringstream temp;
	temp << _port;

	int status = getaddrinfo(NULL, temp.str().c_str(), &hints, &servInfo);
	if (status != 0)
	{
		throw std::runtime_error("init: getaddrinfo() failed");
	}

	for (tmp = servInfo; tmp != NULL; tmp = tmp->ai_next)
	{
		_servSocketFd = socket(tmp->ai_family, tmp->ai_socktype, tmp->ai_protocol);
		if (_servSocketFd == -1)
			continue;
		if (setsockopt(_servSocketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) //make port reusable in case of restart etc.
		{
			close(_servSocketFd);
			continue ;
		}
		if (bind(_servSocketFd, tmp->ai_addr, tmp->ai_addrlen) == -1) //bind the socket to an interface
		{
			close(_servSocketFd);
			continue ;
		}
		break ;
	}
	freeaddrinfo(servInfo);
	if (tmp == NULL)
	{
		throw std::runtime_error("failed to find suitable socket");
	}

	if (fcntl(_servSocketFd, F_SETFL, O_NONBLOCK) == -1) //make actions on port nonblocking
	{
		close(_servSocketFd);
		throw std::runtime_error("init: setting socket option O_NONBLOCK failed: " + std::string(std::strerror(errno)));
	}
	if (listen(_servSocketFd, SOMAXCONN) == -1) //set backup queue of the server to max value
	{
		close(_servSocketFd);
		throw std::runtime_error("init: setting listen() on server socket failed: " + std::string(std::strerror(errno)));
	}
	
	ServPoll.fd = _servSocketFd;
	ServPoll.revents = 0;
	ServPoll.events = POLLIN;

	_fdvec.push_back(ServPoll);

	std::cout << BOLD_GREEN << "Server Started." << RESET << std::endl;
	std::cout << BLUE << "Server port is: " << _port << RESET << std::endl;
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
	if (_clients.size() == _maxClients)
	{
		char full_server[24] = "ERROR :Server is full\r\n";
		send(client_sock, full_server, 23, 0);
		close(client_sock);
		return (-1);
	}
	struct pollfd client_pollfd;

	client_pollfd.revents = 0;
	client_pollfd.fd = client_sock;
	client_pollfd.events = POLLIN | POLLOUT;

	new_fd.push_back(client_pollfd);
	_clients.insert(std::pair<int, Client>(client_sock, Client(client_sock)));
	std::cout << BLUE << "Client id: " << client_sock << " connected" << RESET << std::endl;
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
	newMap["QUIT"] = 9;
	newMap["PART"] = 10;
	newMap["OPER"] = 11;

	return (newMap);
}

void	Server::commands(std::string message, int fd)
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
				command_pass(msg, fd); break;
			case 1:
				command_nick(msg, fd); break;
			case 2:
				command_user(msg, fd); break;
			case 3:
				command_join(msg, fd); break;
			case 4:
				command_privmsg(msg, fd); break;
			case 5:
				command_kick(msg, fd); break;
			case 6:
				command_invite(msg, fd); break;
			case 7:
				command_topic(msg, fd); break;
			case 8:
				command_mode(msg, fd); break;
			case 9:
				command_quit(msg, fd); break;
			case 10:
				command_part(msg, fd); break;
			case 11:
				command_oper(msg, fd); break;
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
	ssize_t rdBytes = recv(it->fd, buffer, 1024, 0);
	if (rdBytes == -1)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return;
		throw std::runtime_error("receiveData: recv() failed");
	}
	if (rdBytes == 0)
	{
		std::cout << BLUE << "Client id: " << it->fd << " disconnected" << RESET << std::endl;
		_clients.erase(it->fd);
		close(it->fd);
		it = _fdvec.erase(it);
		return;
	}
	message.append(buffer, rdBytes);

	Client sourceClient = getClient(it->fd);
	sourceClient.appendMessageBuffer(message);

	if (sourceClient.getMessageBuffer().find("\r\n") != sourceClient.getMessageBuffer().npos)
	{
		//call parsing here
		if (sourceClient.getDisconnect() == true)
		{
			_clients.erase(it->fd);
			close(it->fd);
			it = _fdvec.erase(it);
		}
	}
}

void	Server::sendData(std::vector<struct pollfd>::iterator it)
{
	Client temp = getClient(it->fd);
	
	while (!temp.getSendBuffer().empty())
	{
		errno = 0;
		ssize_t bytes_sent = send(it->fd, temp.getSendBuffer().c_str(), temp.getSendBuffer().size(), MSG_DONTWAIT);
		if (bytes_sent == -1)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				return;
			throw std::runtime_error("sendData: send() failed");
		}
		temp.getSendBuffer().erase(0, bytes_sent);
    }
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
			else if (it->revents & POLLOUT)
			{
				sendData(it);
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
