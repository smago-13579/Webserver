/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smago <smago@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/10 15:20:44 by smago             #+#    #+#             */
/*   Updated: 2021/06/10 19:14:56 by smago            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

# include "webserv.hpp"
# include "kbatwoma/config/Config.hpp"

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
		std::string         	max_body;
	};

	struct Settings
	{
		std::string             ip;
		int                     port;
		std::string             server_name;
		std::string             error_page;
		std::vector<Location>   locations;
	};

	typedef std::vector<Config::Location>::iterator 	config_iter;

	Socket(Config::Server& server);
	~Socket();

	int		accept_client();
	int		create();
	int 	getFD();
	int 	server_read(int fd);

	Settings settings;
	
private:
	struct 	sockaddr_in	addr;
	int 	socket_fd;

};



#endif