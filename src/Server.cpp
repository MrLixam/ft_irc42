#include "../includes/irc_serv.hpp"
#include <iostream>
#include <sys/socket.h>

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

void Server::run(void)
{
	std::cout << "Server Starting ..." << std::endl;
	
}

static void Server::sig_handler(int sig, siginfo_t *info, void *context)
{
	_signal = 1;
}