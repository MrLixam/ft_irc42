/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvincent <lvincent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 17:27:31 by lvincent          #+#    #+#             */
/*   Updated: 2024/07/03 11:49:36 by lvincent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include "../includes/ircserv.hpp"
#include <csignal>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <sys/resource.h>
#include "../includes/colors.hpp"
#include <ctime>

bool server_signal = false;

int maxProcfd(void)
{
	std::ifstream myfile("/proc/self/limits");
	if (!myfile.is_open())
	{
		std::cout << RED << "Could not get file descriptor limit" << RESET << std::endl;
		return (-1);
	}
	std::string buffer;
	int i = -1;
	while (std::getline(myfile, buffer))
	{
		if (buffer.find("Max open files") != buffer.npos)
		{
			std::string word;
			std::istringstream iss(buffer);
			while (iss >> word)
			{
				std::istringstream isInt(word);
				if (isInt >> i)
					break ;
			}
			break ;
		}
	}
	myfile.close();
	return (i);
}

void sig_handler(int sig)
{
	(void)sig;
	server_signal = true;
}

int main(int argc, char **argv)
{
	signal(SIGINT, sig_handler);
	signal(SIGQUIT, sig_handler);

	if (argc != 3)
	{
		std::cout << "Invalid number of arguments, Correct usage as follows:" << std::endl;
		std::cout << "./ircserv <port> <password>" << std::endl;
		return (1);
	}

	if (parseArgs(argv))
	{
		std::cout << RED << "Invalid arguments" << RESET << std::endl;
		return (1);
	}
	int maxFd = maxProcfd();
	if (maxFd < 0)
	{
		std::cout << RED << "Error while getting server limits, aborting" << RESET << std::endl;
		return (1);
	}
	maxFd -= 4;
	if (maxFd <= 0)
	{
		std::cout << RED << "Not enough file descriptors available to process" << RESET << std::endl;
		return (1);
	}
	time_t tmp_time;

	time(&tmp_time);
	struct tm *timeinfo;

	timeinfo = localtime(&tmp_time);
	Server irc_serv(strtol(argv[1], NULL, 10), argv[2], timeinfo);
	irc_serv.setMaxClients((size_t)maxFd);
	try
	{
		irc_serv.init();
		irc_serv.run();
	}
	catch (std::exception &e)
	{
		std::cerr << RED << "Error: " << e.what() << RESET << std::endl;
	}
	return (0);
}
