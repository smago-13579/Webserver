/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbatwoma <kbatwoma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/02 12:45:09 by smago             #+#    #+#             */
/*   Updated: 2021/06/23 11:58:43 by kbatwoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/******************/
/*                */
/*   Exceptions   */
/*                */
/******************/
const char* Server::Abort_server::what() const throw()
{ return ("The Server Core couldn't be started"); }


/******************/
/*                */
/*     Server     */
/*                */
/******************/
Server::Server(std::string str) 
{
	config = new Config(str);
	remove("html/cgi-bin/for_game.tmp");

    std::vector<Config::Server> serv = config->getServers();
	for (conf_iter it = serv.begin(); \
			it != serv.end(); it++)
	{
		Socket socket(*it);
		if (socket.create() == 0)
			servers.insert(std::make_pair(socket.getFD(), socket));
	}
	if (servers.empty()) {
		throw Server::Abort_server();
	}
};

void	Server::server_run()
{
	int res, max_fd = 0;
	std::vector<int> message;
	struct timeval time;
	time.tv_sec = 1;
	time.tv_usec = 0;

	for (;;)
	{
		FD_ZERO(&readfds);
		FD_ZERO(&writefds);
		for (serv_iter it = servers.begin(); it != servers.end(); it++) {
			FD_SET(it->first, &readfds);
			if (it->first > max_fd)
				max_fd = it->first;
		}

		for (cls_iter it = clients.begin(); it != clients.end(); it++) {
			FD_SET(it->first, &readfds);
			if (it->first > max_fd)
				max_fd = it->first;
		}

		for (size_t i = 0; i < message.size(); i++)
			FD_SET(message[i], &writefds);
		
		res = select(max_fd + 1, &readfds, &writefds, NULL, &time);
		if (res == -1) 
		{
			std::string str =  "SELECT ERROR: ";
			str += strerror(errno);
			std::cout << str << std::endl;
			for (cls_iter it = clients.begin(); it != clients.end(); it++)
				it->second->close_socket(it->first);
			clients.clear();
			message.clear();
			continue;
		}
		for (std::vector<int>::iterator it = message.begin(); \
				it != message.end(); it++) 
		{
			if (FD_ISSET(*it, &writefds)) {
				int fd = *it;
				int i = clients[fd]->socket_write(fd);
				if (i != 2)				// (i == 2) if reply not fully sent
				{
					message.erase(it);
					if (i == -1 || i == 1) {			// in case of error OR connection=close
						clients[fd]->close_socket(fd);
						clients.erase(fd);
					}
				}
				break;
			}
		}
		for (cls_iter it = clients.begin(); it != clients.end(); it++)
		{
			if (FD_ISSET(it->first, &readfds)) {
				int i = it->second->socket_read(it->first);
				if (i == -1)
					clients.erase(it->first);
				else if (i == 0)
					message.push_back(it->first);
				break;
			}
		}
		for (serv_iter it = servers.begin(); it != servers.end(); it++) 
		{
			if (FD_ISSET(it->first, &readfds)) {
				int fd = it->second.accept_client();
				if (fd != -1) {
					clients.insert(std::make_pair(fd, &(it->second)));
				}
				break;
			}
		}
	}
}

Server::~Server() 
{
	delete(config);
	for (serv_iter it = servers.begin(); it != servers.end(); it++)
	{
		close(it->first);
	}
};
