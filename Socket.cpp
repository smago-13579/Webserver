/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smago <smago@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/10 15:27:39 by smago             #+#    #+#             */
/*   Updated: 2021/06/11 22:19:30 by smago            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket(Config::Server& server)
{
	settings.ip = server.ip;
	settings.port = server.port;
	settings.server_name = server.server_name;
	settings.error_page = server.error_page;
	
	std::vector<Socket::Location> vec;
	for (config_iter it = server.locations.begin(); it != server.locations.end(); it++)
	{
		Socket::Location lock;
		lock.location = it->location;
		lock.index = it->index;
		lock.methods = it->methods;
		lock.root = it->root;
		lock.autoindex = it->autoindex;
		lock.max_body = it->max_body;
		vec.push_back(lock);
	}	
}

Socket::~Socket() {}

int		Socket::accept_client()
{
	struct sockaddr_in new_addr;
	// struct Request req("");
	socklen_t new_addrlen = sizeof(sockaddr_in);
	int new_fd;

	/*		ACCEPT CONNECTION				*/
	if ((new_fd = accept(socket_fd, (struct sockaddr*)&new_addr, &new_addrlen)) < 0) {
		std::string str =  "ACCEPT ERROR: ";
		str += strerror(errno);
		std::cout << str << std::endl;
	}
	else {
		std::cout << "\n----CONNECTION ACCEPTED----\n";
		std::cout << "Client's FD: " << new_fd << "\tip: " << settings.ip << \
				"\tport: " << settings.port << std::endl;
		// ???			???			???
		// fcntl(new_fd, F_SETFL, O_NONBLOCK);
	}
	return new_fd;
}

int		Socket::create()
{
	/*		CREATE SOCKET 					*/
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	
	if (socket_fd < 0) {
		std::string str = "CAN'T CREATE SOCKET: ";
		std::cout << str << strerror(errno);
		return (1);
	}
	else {
		std::cout << "\n----SOCKET CREATED----\n";
		std::cout << "Socket fd: " << socket_fd << \
			"\tip: " << settings.ip << "\tport: " << settings.port << std::endl;
	}

	/*		STICKING TCP PORT				*/
	int opt = 1;
	setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));	

	/*		CREATE IP AND PORT				*/
	addr.sin_family = AF_INET;
	addr.sin_port = htons(settings.port);
	// addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_addr.s_addr = inet_addr(settings.ip.c_str());

	/*		BIND ADDRESS					*/
	if (bind(socket_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
		std::string str = "BIND ERROR: ";
		std::cout << str << strerror(errno) << std::endl;
		return (1);
	}

	/*		CHECK SUBJECT 9 POINT			*/
	/*		SET NON_BLOCK TO SERVER			*/
	fcntl(socket_fd, F_SETFL, O_NONBLOCK);

	/*		LISTEN MODE						*/
	if (listen(socket_fd, 16) < 0) {		// the Queue could be much longer (about 1000)
		std::string str = "LISTEN ERROR: ";
		std::cout << str << strerror(errno);
		return (1);
	}
	return (0);
}

int 	Socket::getFD()
{
	return (this->socket_fd);
}

int 	Socket::server_read(int fd)
{
	int res;
	char buffer[32648];		
	std::string str;		
	res = recv(fd, buffer, 32647, 0);
	
	if (res > 0) {
		buffer[res] = '\0';

		std::cout << "\nREAD FROM CLIENT: " << fd << std::endl;
		std::cout << buffer << std::endl;
		str = buffer;
		Request tmp(str);
		// req.insert(std::make_pair(fd, tmp));
		// if (req[fd]._request_done == 1)
		// {
			// Response tmp(req[fd], settings);
			// resp.insert(std::make_pair(fd, tmp));
			// std::cout << "REQUEST DONE\n";
			// return (0);
		// }
		// req.insert(std::make_pair(fd, &tmp));
		// if (tmp._request_done)

		return 1;
	}
	else if (res < 0) {
		std::string str = "ERROR WHEN READING FROM THE CLIENT'S FD: ";
		str += itoa(fd);
		str += "\t";
		str += strerror(errno);
		std::cout <<  str << std::endl;
	}
	return 0;
}

// int 	Socket::server_write(int fd)
// {
// 	std::string response;

// 	response = resp[fd].get_response();
// 	if (response != "")
// 	{
// 		if (send(fd, response.c_str(), response.length(), 0) < 0) 
// 		{
// 			std::string str = "ERROR WHEN WRITING TO CLIENT'S SOCKET FD ";
// 			str += itoa(fd);
// 			str += ": ";
// 			str += strerror(errno);
// 			return (1);
// 		}
// 		return (0);
// 	}
	
// 	return (1);
// }

