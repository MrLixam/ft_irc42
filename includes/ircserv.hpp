/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvincent <lvincent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 16:23:28 by r                 #+#    #+#             */
/*   Updated: 2024/06/13 14:53:35 by r                ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCSERV_HPP
#define IRCSERV_HPP

#include <list>
#include <string>

#define MAX_CLIENTS 1024

typedef struct structured_message
{
	std::string				nickname;
	std::string				user;
	std::string				host;
	std::string				command;
	std::list<std::string>	params;
}							struct_msg;

bool		isspecial(char c);
void		format_nickname(std::string nick);
struct_msg	structuring_message(std::string message);
int			parseArgs(char **argv);

#endif
