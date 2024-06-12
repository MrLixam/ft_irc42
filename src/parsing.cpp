/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvincent <lvincent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 17:27:35 by lvincent          #+#    #+#             */
/*   Updated: 2024/06/12 15:53:05 by r                ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ircserv.hpp"
#include <cstdlib>
#include <string>
#include <cerrno>
#include <iostream>
#include <sstream>
#include <cctype>

struct_msg structuring_message(std::string message)
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
	{
        if (token[0] == ':')
        {
            std::string remaining;
            std::getline(iss, remaining);
            token = token.substr(1) + remaining;
            msg.params.push_back(token);
            break;
        }
        msg.params.push_back(token);
    }

    return msg;
}

bool	isspecial(char c)
{
	return ((c >= 0x5B && c <= 0x60) || (c >= 0x7B && c <= 0x7D));
}

void	format_nickname(std::string nick)
{
	if (nick.length() > 9)
		throw 432;
	if (!isalpha(nick[0]) && !isspecial(nick[0]))
		throw;
	for (int i = 1; i <= 9; i++)
		if (!isalpha(nick[i]) && !isspecial(nick[i]) && !isdigit(nick[i]) && nick[i] != '-')
			throw 432;
}

void	command_nick(struct_msg msg)
{
	if (msg.params.size() < 1)
		throw 461;
	format_nickname(msg.params.front());
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
	
	if (port <= 0)
	{
		std::cout << "port can't be negative or 0" << std::endl;
		return (1);
	}
	
	return (0);
}
