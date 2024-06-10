/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvincent <lvincent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 17:27:35 by lvincent          #+#    #+#             */
/*   Updated: 2024/06/10 16:05:32 by r                ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ircserv.hpp"
#include <cstdlib>
#include <string>
#include <cerrno>
#include <iostream>

stm	structuring_message(std::string message)
{
	struct_msg			msg;
    std::istringstream	iss(message);
    std::string			token;

    // Parse prefix
    if (message[0] == ':')
	{
        iss.ignore(1); // ignore the initial ':'
        std::getline(iss, token, ' ');
        size_t exclam = token.find('!');
        size_t at = token.find('@');
        if (exclam != std::string::npos)
		{
            msg.nickname = token.substr(0, exclam);
            msg.user = token.substr(exclam + 1, at - exclam - 1);
            msg.host = token.substr(at + 1);
        }
		else if (at != std::string::npos)
		{
            msg.nickname = token.substr(0, at);
            msg.host = token.substr(at + 1);
        }
		else
            msg.nickname = token;
    }

    // Parse command
    iss >> msg.command;

    // Parse params
    while (iss >> token)
        msg.params.push_back(token);

    return msg;
}

int parseArgs(char **argv)
{
	errno = 0;
	char *end;
	int	port = strtol(argv[1], &end, 10);

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
