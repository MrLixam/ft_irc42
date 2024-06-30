/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvincent <lvincent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 00:30:14 by lvincent          #+#    #+#             */
/*   Updated: 2024/06/30 18:05:05 by gpouzet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_HPP
#define REPLIES_HPP


#define user_id(nickname, username) (nickname + "!" + username + "@localhost")
#define RPL_WELCOME(nickname, user_id) (":42IRC 001 " + nickname + " :Welcome to the Internet Relay Network " + ":" + user_id + "\r\n")
#define RPL_YOURHOST(nickname, servername, version) (":42IRC 002 " + nickname + " :Your host is " + servername + " running version " + version + "\r\n")
#define RPL_CREATED(nickname, time) (":42IRC 003 " + nickname + " :This server was created " + time)
#define RPL_MYINFO(nickname, servername, version, available_user_modes, available_channel_modes) (":42IRC 004 " + nickname + " " + servername + " " + version + " " + available_user_modes + " " + available_channel_modes + "\r\n")
#define RPL_ISUPPORT(nickname, params) (":42IRC 005 " + nickname + " " + params + " :are supported by this server" + "\r\n")
#define RPL_TOPIC(channel, topic) (":42IRC 332 " + channel + " :" + topic + "\r\n")
#define RPL_NAMREPLY(channel, nickname, nick_list) (":42IRC 353 " + nickname + " = " + channel + " :" + nick_list + "\r\n")
#define RPL_ENDOFNAMES(channel) (":42IRC 366 " + channel + " :End of NAMES list\r\n")
#define RPL_INVITING(nickname, invited_nick, channel) (":42IRC 341 " + nickname + " " + invited_nick + " " + channel + "\r\n")

#define JOIN_RPL(user_id, channel) (":" + user_id + " JOIN :" + channel + "\r\n")
#define PART_RPL(user_id, channel, message) (":" + user_id + " PART #" + channel + " " + message + "\r\n")
#define	PART_RPL_NOMSG(user_id, channel) (":" + user_id + " PART #" + channel + "\r\n")
#define QUIT_RPL(user_id, message) (":" + user_id + " QUIT :" + message + "\r\n")
#define PONG_RPL(user_id, message) (":" + user_id + " PONG :" + message + "\r\n")
#endif
