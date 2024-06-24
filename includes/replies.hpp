/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvincent <lvincent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 00:30:14 by lvincent          #+#    #+#             */
/*   Updated: 2024/06/25 00:43:09 by lvincent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_HPP
#define REPLIES_HPP


#define user_id(nickname, username) (":" + nickname + "!" + username + "@localhost")
#define RPL_WELCOME(nickname, user_id) (":42IRC 001 " + nickname + " :Welcome to the Internet Relay Network " + user_id + "\r\n")

#endif