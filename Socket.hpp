/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smago <smago@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/10 15:20:44 by smago             #+#    #+#             */
/*   Updated: 2021/06/11 22:20:48 by smago            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

# include "webserv.hpp"
# include "Request.hpp"
# include "kbatwoma/config/Config.hpp"

# define GET 0
# define POST 1
# define DELETE 2

# define ON 1
# define OFF 0

class Socket
{
public:
	struct Location
	{
		std::string         	location;
		std::string         	index;
		std::vector<size_t> 	methods;
		std::string         	root;
		bool                	autoindex;
		int			         	max_body;
		/*... 	Add another variables			*/
	};

	struct Settings
	{
		std::string             ip;
		int                     port;
		std::string             server_name;
		std::string             error_page;
		std::vector<Location>   locations;

		Settings&			operator=(Settings& set)
		{
			if (this != &set)
			{
				this->ip = set.ip;
				this->port = set.port;
				this->server_name = set.server_name;
				this->error_page = set.error_page;
				this->locations = set.locations;
			}
			return (*this);
		}
	};

	typedef std::vector<Config::Location>::iterator 	config_iter;

	Socket(Config::Server& server);
	~Socket();

	int		accept_client();
	int		create();
	int 	getFD();
	int 	server_read(int fd);
	// int 	server_write(int fd);

	Settings settings;
	
private:
	// std::map<int, class Response> resp;
	// std::map<int, class Request> req;
	struct 	sockaddr_in	addr;
	int 	socket_fd;

};

// # include "Response.hpp"


#endif