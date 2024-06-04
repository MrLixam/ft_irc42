/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvincent <lvincent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 17:27:35 by lvincent          #+#    #+#             */
/*   Updated: 2024/06/04 17:27:42 by lvincent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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