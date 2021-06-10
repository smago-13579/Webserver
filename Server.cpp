/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smago <smago@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/02 12:45:09 by smago             #+#    #+#             */
/*   Updated: 2021/06/10 19:30:13 by smago            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(std::string str) 
{
	config = new Config(str);
	
    std::vector<Config::Server> serv = config->getServers();
	for (conf_iter it = serv.begin(); \
			it != serv.end(); it++)
	{
		Socket socket(*it);
		if (socket.create() == 0)
			servers.insert(std::make_pair(socket.getFD(), socket));
	}
	if (servers.empty()) {
		std::string str = "The Server Core couldn't be started\n";
		throw str;
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
				close(it->first);
			clients.clear();
			message.clear();
			continue;
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
		for (cls_iter it = clients.begin(); it != clients.end(); it++)
		{
			if (FD_ISSET(it->first, &readfds)) {
				if (it->second->server_read(it->first) == 0) {
					std::cout << "socket closed: " << it->first << std::endl;
					clients.erase(it->first);
				}
				message.push_back(it->first);
				break;
			}
		}
	}
}


Server::~Server() 
{
	delete(config);
};

