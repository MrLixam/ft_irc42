#include "../includes/ircserv.hpp"
#include <cstdlib>
#include <string>
#include <cerrno>
#include <iostream>

int parseArgs(char **argv)
{
	char *end;

	errno = 0;
	int port = strtol(argv[1], &end, 10);
	if (*end != '\0' || errno)
	{
		std::cout << "Port should be an integer" << std::endl;
		return (1);
	}
	if (port < 0)
	{
		std::cout << "port can't be negative" << std::endl;
		return (1);
	}
	return (0);
}