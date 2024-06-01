#include "../includes/ircserv.hpp"
#include "../includes/Server.hpp"
#include <arpa/inet.h>
#include <iostream>
#include <poll.h>
#include <sys/socket.h>
#include <stdexcept>
#include <fcntl.h>

Server::Server(void)
{
	_signal = 0;
	_port = 4444;
	_password = "";
}

Server::Server(int port, std::string password)
{
	_signal = 0;
	_port = port;
	_password = password;
	std::cout << "Server initialised !" << std::endl;
}

Server::~Server(void)
{
	std::cout << "Server Shutdown" << std::endl;
}

void Server::init(void)
{
	std::cout << "Server Starting ..." << std::endl;
	
	pollfd ServPoll;
	sockaddr_in addr;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(_port);
	addr.sin_addr.s_addr = INADDR_ANY;

	_servSocketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_servSocketFd == -1)
		throw std::runtime_error("socket creation failed");
	
	int opt = 1;
	if (setsockopt(_servSocketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
		throw std::runtime_error("init: setting socket option SO_REUSEADDR failed");
	if (fcntl(_servSocketFd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("init: setting socket option O_NONBLOCK failed");
	if (bind(_servSocketFd, (sockaddr *)&addr, sizeof(addr)) == -1)
		throw std::runtime_error("init: setting bind() on server socket failed");
	if (listen(_servSocketFd, SOMAXCONN) == -1)
		throw std::runtime_error("init: setting listen() on server socket failed");
	
	ServPoll.fd = _servSocketFd;
	ServPoll.revents = 0;
	ServPoll.events = POLLIN;

	fdvec.push_back(ServPoll);

	std::cout << "Server Started." << std::endl;
}


void Server::run(void)
{
	while (_signal == false)
	{
		if (poll(fdvec.begin().base(), fdvec.size(), -1) == -1)
		{
			if (_signal == false)
				throw std::runtime_error("run: poll() failed");
			return ;
		}
		for (std::vector<pollfd>::iterator it = fdvec.begin(); it != fdvec.end(); it++)
		{
			if (it->revents == 0)
				continue;
			if (it->revents & POLLIN)
			{
				if (it->fd == _servSocketFd)
					//ici pour quoi faire quand un client se connecte;
					continue ;
				else
					//ici pour recevoir data
					continue ;
			}
			if (it->revents & POLLHUP)
			{
				//ici pour deco d'un client :)s
				continue;
			}
		}
	}
	//deco du serv, il faudra fermer les fd, etc..
}

void Server::sig_handler(int sig)
{
	_signal = true;
}
