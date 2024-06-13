/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: r <marvin@42.fr>                           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 15:29:36 by r                 #+#    #+#             */
/*   Updated: 2024/06/13 15:56:44 by r                ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

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
