#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <set>
#include "Client.hpp"

class Channel
{
	private:
		std::set<Client> _clients;
};

#endif