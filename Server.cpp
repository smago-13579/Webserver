/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smago <smago@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/02 12:45:09 by smago             #+#    #+#             */
/*   Updated: 2021/06/03 17:57:04 by smago            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server() 
{
	int flags;

	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd < 0) {
		std::string str = "CAN'T CREATE SOCKET: ";
		str += strerror(errno);
		throw str;
	}
	else {
		std::cout << "\n----SOCKET CREATED----\n";
		std::cout << "Socket fd: " << socket_fd << std::endl;
	}	

	/*		STICKING TCP PORT				*/
	int opt = 1;
	setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));	

	/*		CREATE IP AND PORT				*/
	addr.sin_family = AF_INET;
	addr.sin_port = htons(7654);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	/*		BIND ADDRESS					*/
	if (bind(socket_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
		std::string str = "BIND ERROR: ";
		str += strerror(errno);
		throw str;
	}

	/*		CHECK SUBJECT 9 POINT			*/
	/*		SET NON_BLOCK TO SERVER			*/
	// flags = fcntl(socket_fd, F_GETFL);
	// fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK);

	/*		LISTEN MODE						*/
	if (listen(socket_fd, 16) < 0) {
		std::string str = "LISTEN ERROR: ";
		str += strerror(errno);
		throw str;
	}
};

void	Server::accept_client()
{
	struct sockaddr_in new_addr;
	socklen_t new_addrlen = sizeof(sockaddr_in);
	int new_fd;

	/*		ACCEPT CONNECTION				*/
	if ((new_fd = accept(socket_fd, (struct sockaddr*)&new_addr, &new_addrlen)) < 0) {
		std::string str =  "ACCEPT ERROR: ";
		str += strerror(errno);
		throw str;
	}
	else {
		std::cout << "\n----CONNECTION ACCEPTED----\n";
	/* 		the number of bytes that the call wrote to struct new_addr and new FD	*/
		std::cout << "The number of bytes that the call wrote: " << new_addrlen << std::endl; 
		std::cout << "FD of the client's socket: " << new_fd << std::endl;
	}
	this->FDs.push_back(new_fd);
}

int 	Server::server_read(int fd)
{
	int res;
	char buffer[1024];
	res = recv(fd, buffer, 1023, 0);
	if (res > 0) {
		buffer[res] = '\0';

		std::cout << "\nREAD FROM CLIENT: " << fd << std::endl;
		std::cout << buffer << std::endl;
		return 1;
	}
	else if (res < 0) {
		std::string str = "ERROR WHEN READING FROM THE CLIENT'S SOCKET: ";
		str += strerror(errno);
		throw str;
	}
	return 0;
}

void	Server::server_run()
{
	int maxfd, res;
	fd_set readfds, writefds;

	for (;;) 
	{
		maxfd = socket_fd;
		FD_ZERO(&readfds);
		FD_ZERO(&writefds);
		FD_SET(socket_fd, &readfds);
		for (int i = 0; i < FDs.size(); i++)
		{
			FD_SET(FDs[i], &readfds);
			if (FDs[i] > maxfd) 
				maxfd = FDs[i];
		}
		res = select(maxfd + 1, &readfds, NULL, NULL, NULL);
		if (res == -1) 
		{
			std::string str =  "SELECT ERROR: ";
			str += strerror(errno);
			throw str;
		}
		if (FD_ISSET(socket_fd, &readfds)) {
			accept_client();
		}
		for (int i = 0; i < FDs.size(); i++) 
		{
			if (FD_ISSET(FDs[i], &readfds)) {
				if (!server_read(FDs[i])) {
					std::cout << "socket closed: " << FDs[i] << std::endl;
					close(FDs[i]);
					FDs.erase(FDs.begin() + i);
				}
			}
		}
	}
}


Server::~Server() 
{
	close(socket_fd);
};

