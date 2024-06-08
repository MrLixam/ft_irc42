/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvincent <lvincent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 17:27:31 by lvincent          #+#    #+#             */
/*   Updated: 2024/06/08 10:52:23 by lvincent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	
	Server irc_serv(strtol(argv[1], NULL, 10), argv[2]);
	try
	{
		irc_serv.init();
		irc_serv.run();
	}
	catch (std::exception &e)
	{
		std::cout << "Error: " << e.what() << std::endl;
	}
	
	std::cout << "Server has Shutdown." << std::endl;
	return (0);
}