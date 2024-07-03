/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_BNF.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvincent <lvincent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 18:47:25 by r                 #+#    #+#             */
/*   Updated: 2024/07/03 12:00:25 by gpouzet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ircserv.hpp"
#include <cstdlib>
#include <string>
#include <iostream>
#include <sstream>
#include <netdb.h>
#include <cstring> 
#include <cctype>

bool	isspecial(char c)
{
	return ((c >= 0x5B && c <= 0x60) || (c >= 0x7B && c <= 0x7D));
}

void	format_nickname(std::string client, std::string nick)
{
	if (nick.length() > 9)
		throw ERR_ERRONEUSNICKNAME(client + " " + nick);
	if (!isalpha(nick[0]) && !isspecial(nick[0]))
		throw ERR_ERRONEUSNICKNAME(client + " " + nick);
	for (int i = 1; nick[i] && i <= 9; i++)
		if (!isalpha(nick[i]) && !isspecial(nick[i]) && !isdigit(nick[i]) && nick[i] != '-')
			throw ERR_ERRONEUSNICKNAME(client + " " + nick);
}

bool    format_user(std::string user)
{
    for (size_t i = 0; i < user.length(); i++)
	{
		char c = user[i];
		if (!((c >= 0x01 && c <= 0x09) ||
              (c >= 0x0B && c <= 0x0C) ||
              (c >= 0x0E && c <= 0x1F) ||
              (c >= 0x21 && c <= 0x3F) ||
              c >= 0x41))
            return (false);
	}
    return (true);
}

bool	format_shortname(std::string name)
{
	if (!isalpha(name[0]) && !isdigit(name[0]))
		return (false);
	for (size_t i = 1; i < name.length() - 1; i++)
		if (!isalpha(name[i]) && !isdigit(name[i]) && name[i] != '-')
			return (false);
	if (!isalpha(name[name.length() - 1]) && !isdigit(name[name.length() - 1]))
		return (false);
    return (true);
}

bool	format_hostname(std::string name)
{
	std::stringstream   ss(name);
	std::string token;
	while (getline(ss, token, '.'))
		if (!format_shortname(token))
			return (false);
	return (true);
}

bool	format_chanstring(std::string chanstring)
{
	for (size_t i = 0; i < chanstring.length(); ++i) 
	{
		char c = chanstring[i];
		if (!((c >= 0x01 && c <= 0x07) ||
              (c >= 0x08 && c <= 0x09) ||
              (c >= 0x0B && c <= 0x0C) ||
              (c >= 0x0E && c <= 0x1F) ||
              (c >= 0x21 && c <= 0x2B) ||
              (c >= 0x2B && c <= 0x39) ||
              c >= 0x3B))
            return (false);
	}
	return (true);
}

bool	format_channel(std::string chan)
{
	if (chan[0] != '#' && chan[0] != '&')
		return (false);
    std::string chanstring;
	size_t colonPos = chan.find(':');
    if (colonPos != std::string::npos)
	{
        chanstring = chan.substr(1, colonPos - 1);
        std::string optionalChanstring = chan.substr(colonPos + 1);
		if (optionalChanstring.empty() || !format_chanstring(optionalChanstring))
			return (false);
	}
	else
        chanstring = chan.substr(1);
	if (chanstring.empty() || !format_chanstring(chanstring))
		return (false);
	return (true);
}

bool	msgto_user(std::string message)
{
	std::string::size_type atPos = message.find('@');
	std::string::size_type perPos = message.find('%');
	std::string user;
    if (atPos != std::string::npos)
	{
		user = message.substr(0, atPos);
    	std::string servername = message.substr(atPos + 1);
		if (!servername.empty() && !format_hostname(servername))
			return (false);
    	if (perPos != std::string::npos)
		{
    		std::string host = user.substr(perPos + 1);
			user = user.substr(0, atPos);
			if (!host.empty() && !format_hostname(host))
				return (false);
		}
	}
	else if (perPos != std::string::npos)
	{
		user = message.substr(0, perPos);
    	std::string host = message.substr(perPos + 1);
		if (!host.empty() && !format_hostname(host))
			return (false);
	}
	else
		return (false);
	if (!user.empty() && !format_user(user))
		return (false);
	return (true);
}

bool	msgto_nickname(std::string nick)
{
	try
	{
		format_nickname("*", nick);
	}
	catch (commandException& e)
	{
		return (false);
	}
	return (true);
}
