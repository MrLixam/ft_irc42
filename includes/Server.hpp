#ifndef SERVER_HPP
#define SERVER_HPP

#include <set>
#include <string>
#include <Client.hpp>

class Server
{
	private:
		std::set<Client> _clients; //list of connected clients, std::set used for better lookup times
		
		int	_port;			
		std::string	_password;	//arguments given as input.

		bool _signal; //used to shutdown the server when signals are received

	public:
		static void sig_handler(int sig, siginfo_t *info, void *context);
		Server();
		Server(int port, std::string password);
		~Server();
		void run();
		void newClient();
		void receiveData(int fd);
};

#endif