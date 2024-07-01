/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvincent <lvincent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 17:27:40 by lvincent          #+#    #+#             */
/*   Updated: 2024/07/01 17:41:08 by lvincent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ircserv.hpp"
#include "../includes/Server.hpp"
#include "../includes/colors.hpp"
#include "../includes/replies.hpp"
#include <fcntl.h>
#include <ctime>
#include <sys/time.h>
#include <cstring>
#include <sstream>
#include <cerrno>
#include <cstdio>
#include <unistd.h>

Server::Server(void)
{
	_port = 4444;
	_password = "";
}

Server::Server(int port, std::string password, struct tm * timeinfo)
{
	_port = port;
	_password = password;
	
	char buffer[80];

	strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", timeinfo);

	std::string tmp(buffer);
	_creationDate = tmp;
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

	if (tmp == NULL)
	{
		freeaddrinfo(servInfo);
		throw std::runtime_error("failed to find suitable socket");
	}
	freeaddrinfo(servInfo);
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

void Server::newClient(std::vector<struct pollfd>& new_fd)
{
	struct sockaddr_in clientaddr;
	socklen_t size = sizeof(struct sockaddr_in);

	int client_sock = accept(_servSocketFd, (struct sockaddr *)&clientaddr, &size);
	if (client_sock == -1)
	{
		std::cout << "newClient(): accept() failed" << std::endl;
		return ;
	}
	if (fcntl(client_sock, F_SETFL, O_NONBLOCK) == -1)
	{
		close(client_sock);
		throw std::runtime_error("newClient: setting socket option O_NONBLOCK failed");
	}
	if (_clients.size() == _maxClients)
	{
		char full_server[24] = "ERROR :Server is full\r\n";
		send(client_sock, full_server, 23, 0);
		close(client_sock);
		return ;
	}
	struct pollfd client_pollfd;

	client_pollfd.revents = 0;
	client_pollfd.fd = client_sock;
	client_pollfd.events = POLLIN | POLLOUT;

	new_fd.push_back(client_pollfd);
	Client newClient(client_sock);
	_clients[client_sock] = newClient;
	std::cout << BLUE << "Client id: " << client_sock << " connected" << RESET << std::endl;
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
	newMap["CAP"] = 11;
	newMap["PING"] = 12;
	newMap["NAMES"] = 13;

	return (newMap);
}

void	Server::commands(std::string message, int fd)
{

	static std::map<std::string, int> CommandMap = initCmdMap();
	struct_msg	msg = structuring_message(message);

	Client& tmp = _clients[fd];
	std::map<std::string, int>::iterator it = CommandMap.find(msg.command);

	int commandSwitch = (it != CommandMap.end()) ? it->second : -1;
	try
	{
		if (msg.command.empty())
			throw ERR_UNKNOWNCOMMAND(tmp.getNickname());
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
			case 11: //CAP command, we don't implement it but it avoids littering output with useless error codes on client connection
				break;
			case 12:
				command_ping(msg, fd); break;
			case 13:
				command_name(msg, fd); break;
			default:
				throw ERR_UNKNOWNCOMMAND(tmp.getNickname());
		}

	}
	catch (commandException& e)
	{
		std::cout << RED << "commands: send error code " << e._errorCode << RESET << std::endl;
		std::ostringstream convert;
		convert << e._errorCode;
		std::string message = ":42IRC " + convert.str() + " " + e._errorMessage + "\r\n";
		tmp.appendSendBuffer(message);
	}

	if (!tmp.getRegistered() && tmp.getPass() && !tmp.getNickname().empty() && !tmp.getRealname().empty())
	{
		messageToClient(tmp.getFd(), RPL_WELCOME(tmp.getNickname(), user_id(tmp.getNickname(), tmp.getUsername())));
		messageToClient(tmp.getFd(), RPL_YOURHOST(tmp.getNickname(), "42IRC", "1.0"));
		messageToClient(tmp.getFd(), RPL_CREATED(tmp.getNickname(), _creationDate));
		messageToClient(tmp.getFd(), RPL_MYINFO(tmp.getNickname(), "42IRC", "1.0", ".", "it klo"));
		messageToClient(tmp.getFd(), RPL_ISUPPORT(tmp.getNickname(), "CHANTYPES=&# MODES=3"));
		messageOfTheDay("Welcome to 42IRC", tmp);
		tmp.setRegistered(true);
	}
}

void Server::receiveData(struct pollfd& it)
{
	char buffer[1024];
	std::string message;

	errno = 0;
	memset(buffer, 0, 1024);
	ssize_t rdBytes = recv(it.fd, buffer, 1024, 0);
	if (rdBytes == -1)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return;
		perror("receiveData: recv() failed");
		_clients[it.fd].setDisconnect(true);
	}
	if (rdBytes == 0)
	{
		_clients[it.fd].setDisconnect(true);
		return;
	}
	message.append(buffer, rdBytes);

	std::cout << "Received message: " << message << std::endl;
	Client& sourceClient = _clients[it.fd];
	sourceClient.appendMessageBuffer(message);

	while (sourceClient.getMessageBuffer().find("\r\n") != sourceClient.getMessageBuffer().npos)
	{
		std::string messageBuffer = sourceClient.getMessageBuffer();
		std::string subMessage = messageBuffer.substr(0, messageBuffer.find("\r\n") + 2);
		
		messageBuffer.erase(0, subMessage.length());
		commands(subMessage, it.fd);
		sourceClient.setMessageBuffer(messageBuffer);
	}
}

void	Server::sendData(struct pollfd& it)
{
	Client& temp = _clients[it.fd];
	
	std::string buff = temp.getSendBuffer();
	while (!temp.getSendBuffer().empty())
	{
		ssize_t bytes_sent = send(it.fd, temp.getSendBuffer().c_str(), temp.getSendBuffer().size(), 0);
		if (bytes_sent == -1)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				return;
		}
		std::string sendBuffer = temp.getSendBuffer();
		sendBuffer.erase(0, bytes_sent);
		temp.setSendBuffer(sendBuffer);
	}
}

void Server::run(void)
{
	_fdvec.reserve(_maxClients);

	while (server_signal == false)
	{
		if (poll(_fdvec.begin().base(), _fdvec.size(), -1) == -1)
		{
			if (server_signal == false)
				throw std::runtime_error("run: poll() failed");
			break ;
		}
		for (size_t i = 0; i < _fdvec.size(); i++)
		{
			struct pollfd& current = _fdvec[i];
			if (current.revents & POLLIN)
			{
				if (current.fd == _servSocketFd)
					newClient(_fdvec);
				else
					receiveData(current);
			}
		}
		for (size_t i = 0; i < _fdvec.size(); i++)
		{
			struct pollfd& current = _fdvec[i];
			if (!_clients[current.fd].getSendBuffer().empty() && current.revents & POLLOUT)
				sendData(current);
			if (_clients[current.fd].getDisconnect() == true)
			{
				_clients.erase(current.fd);
				close(current.fd);
				std::cout << BLUE << "Client id: " << current.fd << " disconnected" << RESET << std::endl;
				_fdvec.erase(_fdvec.begin() + i);
			}
		}
	}
	while(_fdvec.size() != 0)
	{
		close(_fdvec[0].fd);
		_fdvec.erase(_fdvec.begin());
	}
	//deco du serv, il faudra fermer les fd, etc..
}
