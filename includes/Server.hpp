#ifndef SERVER_HPP
#define SERVER_HPP

#include <set>
#include <sys/socket.h>
#include <netdb.h>
#include <csignal>
#include <string>
#include <Client.hpp>
#include <Channel.hpp>

class Server
{
	private:
		std::set<Client> _clients; //list of connected clients, std::set used for better lookup times
		std::set<Channel> _channels;

		int	_port;			
		std::string	_password;	//arguments given as input.

		static bool _signal; //used to shutdown the server when signals are received

		struct addrinfo _hints;
		struct addrinfo _servRes;
			
	public:
		static void sig_handler(int sig);
		Server();
		Server(int port, std::string password);
		~Server();
		int fillServerInfo(void);
		void run();
		void newClient();
		void receiveData(int fd);
};

#endif