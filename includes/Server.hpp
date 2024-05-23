#ifndef SERVER_HPP
#define SERVER_HPP

#include <set>
#include <string>
#include <Client.hpp>

class Server
{
	private:
		std::set<Client> clients; //list of connected clients, std::set used for better lookup times
		
		int	port;			
		std::string	password;	//arguments given as input.

		int SocketFd;

	public:
		static bool Signal; //used to shutdown the server when signals are received ( namely)
		Server();
		Server(int port, std::string password);
		~Server();
		void Run();
		void NewClient();
		void ReceiveData(int fd);
};

#endif