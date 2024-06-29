/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvincent <lvincent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 15:29:36 by r                 #+#    #+#             */
/*   Updated: 2024/06/29 13:55:57 by lvincent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include "../includes/ircserv.hpp"
#include "../includes/colors.hpp"

int Server::usernameExists(const std::string username, int fd) const
{
    for (std::map<int, Client>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (it->second.getFd() != fd)
			if (it->second.getUsername() == username)
				return (it->second.getFd());
    }
    return (-1);
}

Client&	Server::getClient(int fd)
{
	std::map<int, Client>::iterator it = _clients.find(fd);
	if (it == _clients.end()) //le fd n'existe pas dans le serveur
		throw std::runtime_error("getClient: invalid fd"); //peu importe juste de la gestion d'erreur
	Client& tmp = it->second;
	return (tmp);
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


void	Server::messageToClient(int fd, std::string message)
{
	try
	{
		getClient(fd).appendSendBuffer(message);
	}
	catch (std::exception& e)
	{
		std::cerr << RED << "no such fd in server" << RESET << std::endl;
	}
}

std::string Server::clientList(std::set<int>& fdList)
{
	std::string ret;
	for (std::set<int>::iterator it = fdList.begin(); it != fdList.end(); it++)
	{
		ret += getClient(*it).getNickname();
		ret += " ";
	}
	ret.erase(ret.end() - 1);
	return (ret);
}

void Server::messageOfTheDay(std::string message, Client& client)
{
	messageToClient(client.getFd(), ":42IRC 375 " + client.getNickname() + " :- 42IRC Message of the day - " + "\r\n");
	messageToClient(client.getFd(), ":42IRC 372 " + client.getNickname() + " :" + message + "\r\n");
	messageToClient(client.getFd(), ":42IRC 376 " + client.getNickname() + " :End of /MOTD command." + "\r\n");
}
