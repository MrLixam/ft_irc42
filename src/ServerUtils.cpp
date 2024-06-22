/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvincent <lvincent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 15:29:36 by r                 #+#    #+#             */
/*   Updated: 2024/06/22 18:04:55 by lvincent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include "../includes/ircserv.hpp"
#include "../includes/colors.hpp"

std::string getErrorMessage(std::string errorInfo, int errorCode)
{
	switch (errorCode)
	{
		case 401:
			return (errorInfo + " :No such nick/channel");
		case 403:
			return (errorInfo + " :No such channel");
		case 404:
			return (errorInfo + " :Cannot send to channel");
		case 407:
			return (errorInfo + " :407 recipients. Message not delivered");
		case 412:
			return (":No text to send");
		case 421:
			return (errorInfo + " :Unknown command");
		case 431:
			return (":No nickname given");
		case 432:
			return (errorInfo + " :Erroneous nickname");
		case 433:
			return (errorInfo + " :Nickname is already in use");
		case 441:
			return (errorInfo + " :They aren't on that channel");
		case 442:
			return (errorInfo + " :You're not on that channel");
		case 443:
			return (errorInfo + " :is already on channel");
		case 451:
			return (":You have not registered");
		case 461:
			return (errorInfo + " :Not enough parameters");
		case 462:
			return (":Unauthorized command (already registered)");
		case 464:
			return (":Password incorrect");
		case 465:
			return (":You are banned from this server");
		case 466:
			return ("");
		default:
			return ("");
	}
}

bool Server::usernameExists(const std::string username, int fd) const
{
    for (std::map<int, Client>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (it->second.getFd() != fd)
			if (it->second.getUsername() == username)
				return true;
    }
    return false;
}

Client	Server::getClient(int fd)
{
	std::map<int, Client>::iterator it = _clients.find(fd);
	if (it == _clients.end()) //le fd n'existe pas dans le serveur
		throw std::runtime_error("invalid fd"); //peu importe juste de la gestion d'erreur
	return (it->second);
}

void	Server::messageToChannel(std::set<int> fdList, std::string message)
{
	for (std::set<int>::iterator it = fdList.begin(); it != fdList.end(); it++)
	{
		try
		{
			getClient(*it).appendSendBuffer(message);
		}
		catch (std::exception& e)
		{
			std::cerr << RED << "no such fd in server" << RESET << std::endl;
		}
	}
}

void	Server::messageToChannel(std::set<int> fdList, std::string message, int senderFd)
{
	for (std::set<int>::iterator it = fdList.begin(); it != fdList.end(); it++)
	{
		if (*it == senderFd)
			continue ;
		try
		{
			getClient(*it).appendSendBuffer(message);
		}
		catch (std::exception& e)
		{
			std::cerr << RED << "no such fd in server" << RESET << std::endl;
		}
	}
}