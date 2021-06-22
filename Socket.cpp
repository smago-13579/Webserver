/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smago <smago@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/10 15:27:39 by smago             #+#    #+#             */
/*   Updated: 2021/06/22 20:52:57 by smago            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket(Config::Server& server)
{
	settings.ip = server.ip;
	settings.port = server.port;
	settings.server_name = server.server_name;
	settings.error_page = server.error_page;
	settings.redirect = server.redirect;
	
	std::vector<Location> vec;
	for (config_iter it = server.locations.begin(); it != server.locations.end(); it++)
	{
		Location lock;
		lock.location = it->location;
		lock.index = it->index;
		lock.methods = it->methods;
		lock.root = it->root;
		lock.autoindex = it->autoindex;
		lock.max_body = it->max_body;
		lock.exec = it->exec;
		vec.push_back(lock);
	}
	settings.locations = vec;
}

Socket::~Socket() {}

int		Socket::accept_client()
{
	struct sockaddr_in new_addr;
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
		
	/*		SET NON_BLOCK TO CLIENT			*/
		fcntl(new_fd, F_SETFL, O_NONBLOCK);
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
	addr.sin_addr.s_addr = inet_addr(settings.ip.c_str());

	/*		BIND ADDRESS					*/
	if (bind(socket_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
		std::string str = "BIND ERROR: ";
		std::cout << str << strerror(errno) << std::endl;
		close(socket_fd);
		return (1);
	}

	/*		SET NON_BLOCK TO SERVER			*/
	fcntl(socket_fd, F_SETFL, O_NONBLOCK);

	/*		LISTEN MODE						*/
	if (listen(socket_fd, 1000) < 0) {
		std::string str = "LISTEN ERROR: ";
		std::cout << str << strerror(errno);
		close(socket_fd);
		return (1);
	}
	return (0);
}

int 	Socket::getFD()
{
	return (this->socket_fd);
}

void	Socket::erase_request(int fd)
{
	if (req.find(fd) != req.end())
	{
		req.erase(fd);
		std::cout << "REQUEST DELETED\n";
	}
}

int 	Socket::socket_read(int fd)
{
	int res;
	char buffer[100000];		
	std::string str;		
	res = recv(fd, buffer, 99999, 0);
	
	if (res > 0) {
		buffer[res] = '\0';

		std::cout << "\nREAD FROM CLIENT: " << fd << std::endl;
		std::cout << buffer << std::endl;
		str = buffer;
		if (req.find(fd) == req.end())
			req[fd] = Request();
		req[fd].processRequest(str);
		if (req[fd]._request_done == WAITING)
			return 2;
		else if (req[fd]._request_done == OK) 
		{
			Response response(req[fd], settings);
			resp[fd] = response;
		}
		else if (req[fd]._request_done == ERROR)
		{
			Response response(400, settings);
			resp[fd] = response;
		}

		/*			Delete request from client			*/
		req.erase(fd);
		return 0;
	}
	else if (res < 0) {
		std::cout << "ERROR WHEN READING FROM THE CLIENT'S FD: ";
		std::cout << fd << std::endl;
	}
	else
		std::cout << "socket closed: " << fd << std::endl;
	this->close_socket(fd);
	return -1;
}

int 	Socket::socket_write(int fd)
{
	std::string response;
	int res, connection;

	response = resp[fd].get_response();
	if (response != "")
	{
		connection = resp[fd].get_connection();
		// std::cout << "RESPONSE: \n" << response << std::endl;
		if ((res = send(fd, response.c_str(), response.length(), 0)) < 0) 
		{
			std::cout << "ERROR WHEN WRITING TO CLIENT'S SOCKET FD ";
			std::cout << fd << std::endl;
			resp.erase(fd);
			return (-1);
		}
		resp[fd].erase_answer(res);
		if (resp[fd].get_response().empty())
		{
			std::cout << "REQUEST DONE\n";
			resp.erase(fd);
			if (connection == OFF)
				return (1);
			return (0);
		}
		return (2);				// reply not fully sent
	}
	resp.erase(fd);
	return (-1);
}

void	Socket::close_socket(int fd)
{
	close(fd);
	this->erase_request(fd);
}
