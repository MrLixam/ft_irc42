#ifndef SERVER_HPP
#define SERVER_HPP

#include <set>
#include <sys/socket.h>
#include <netdb.h>
#include <csignal>
#include <string>
#include <poll.h>
#include <sys/socket.h>
#include <Client.hpp>
#include <Channel.hpp>
#include <vector>

class Server
{
	private:
		std::set<Client>	_clients; //list of connected clients, std::set used for better lookup times
		std::set<Channel>	_channels;
		std::vector<pollfd>	fdvec;

		int					_port;	
		std::string			_password;	//arguments given as input.

		static bool			_signal; //used to shutdown the server when signals are received

		struct addrinfo		_hints;
		struct addrinfo		_servRes;
		int					_servSocketFd;
			
	public:		
		//Constructors
		Server(void);
		Server(int port, std::string password);
		~Server(void);
		
		//signal handler
		static void	sig_handler(int sig);

		//Getters
		int			getFd(void) const;
		int			getPort(void) const;
		std::string	getPwd(void) const;
		
		//
		void		init(void);
		void		run(void);
		void		newClient();
		void		receiveData(int fd);
};

#endif