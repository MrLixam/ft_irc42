/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvincent <lvincent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 17:27:35 by lvincent          #+#    #+#             */
/*   Updated: 2024/07/02 20:38:51 by gpouzet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ircserv.hpp"
#include "../includes/colors.hpp"
#include <cstdlib>
#include <string>
#include <cerrno>
#include <iostream>
#include <cstring>
#include <sstream>
#include <cctype>
#include <vector>

std::vector<std::string>	parseMode(std::vector<std::string> modestring_lst)
{
    std::vector<std::string> modestring_return;
    for (size_t i = 0; i < modestring_lst.size(); i++)
    {
        std::string modestring = modestring_lst[i];
        for (size_t j = 0; j < modestring.size(); j++)
        {
            if (modestring[j] == '+' || modestring[j] == '-')
            {
                if (j >= modestring.size() - 1)
                    continue ;
                modestring_return.push_back(modestring.substr(j, modestring.find_first_of("-+", j + 1)));
            }
        }
    }
    return (modestring_return);
}

struct_msg	structuring_message(std::string message)
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

std::string	msg_source(struct_msg msg)
{
	std::string	source = "";
	if (!msg.user.empty())
		source += "!" + msg.user;
	if (!msg.host.empty())
		source += "@" + msg.host;
	return (source);
}

int parseArgs(char **argv)
{
	char *end;
	int	port = strtol(argv[1], &end, 10);

	std::string arg1(argv[1]), arg2(argv[2]);

	if (arg1.empty() || arg2.empty())
	{
		std::cout << RED << "You can't put in empty arguments" << RESET << std::endl;
		return (1);
	}

	if (*end != '\0' || errno)
	{
		std::cout << RED << "Port should be an integer" << RESET << std::endl;
		return (1);
	}
	
	if (port <= 0)
	{
		std::cout << RED << "port can't be negative or 0" << RESET << std::endl;
		return (1);
	}
	return (0);
}
