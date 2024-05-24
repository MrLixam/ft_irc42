#include "../includes/Server.hpp"
#include "../includes/ircserv.hpp"
#include <csignal>
#include <iostream>

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cout << "Invalid number of arguments, Correct usage as follows:" << std::endl;
		std::cout << "./ircserv <port> <password>" << std::endl;
		return (1);
	}
	signal(SIGINT, Server::sig_handler);
	signal(SIGTERM, Server::sig_handler);
	if (parseArgs(argv))
	{
		std::cout << "Invalid arguments" << std::endl;
		return (1);
	}
	return (0);
}