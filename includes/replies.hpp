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
#define RPL_TOPIC(channel, topic) (":42IRC 332 " + channel + " :" + topic + "\r\n")
#define RPL_NAMREPLY(channel, nickname, nick_list) (":42IRC 353 " + nickname + " = " + test + " :" + nick_list + "\r\n")
#define RPL_ENDOFNAMES(channel) (":42IRC 366 " + channel + " :End of NAMES list\r\n")
#define RPL_INVITING(nickname, invited_nick, channel) (":42IRC 341 " + nickname + " " + invited_nick + " " + channel + "\r\n")

#endif