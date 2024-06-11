/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvincent <lvincent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 16:23:28 by r                 #+#    #+#             */
/*   Updated: 2024/06/11 11:39:35 by lvincent         ###   ########.fr       */
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

int	parseArgs(char **argv);

#endif
