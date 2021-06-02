/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   primer.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smago <smago@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/24 16:34:02 by smago             #+#    #+#             */
/*   Updated: 2021/06/02 20:51:43 by smago            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void 	primer(void)
{
	/*		CREATE SOCKET FD				*/
	// socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	// if (socket_fd < 0) {
	// 	std::cout << "CAN'T CREATE SOCKET: " << strerror(errno) << std::endl;
	// 	exit(1);
	// }
	// else {
	// 	std::cout << "\n----SOCKET CREATED----\n";
	// 	std::cout << "Socket fd: " << socket_fd << std::endl;
	// }	
	// /*		STICKING TCP PORT				*/
	// opt = 1;
	// setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));	

	// /*		CREATE IP AND PORT				*/
	// addr.sin_family = AF_INET;
	// addr.sin_port = htons(7654);
	// addr.sin_addr.s_addr = htonl(INADDR_ANY);

	// /*		BIND ADDRESS					*/
	// if (bind(socket_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
	// 	std::cout << "BIND ERROR: " << strerror(errno) << std::endl;
	// 	exit(1);
	// }

	// if (listen(socket_fd, 16) < 0) {
	// 	std::cout << "LISTEN ERROR: " << strerror(errno) << std::endl;
	// 	exit(1);
	// }

	// while (1) 
	// {
	// 	maxfd = socket_fd;
	// 	FD_ZERO(&readfds);
	// 	FD_ZERO(&writefds);
	// 	FD_SET(socket_fd, &readfds);
	// 	for (int i = 0; i < FDs.size(); i++)
	// 	{
	// 		FD_SET(FDs[i], &readfds);
	// 		if (FDs[i] > maxfd) 
	// 			maxfd = FDs[i];
	// 	}
	// 	// timeout.tv_sec = 
	// 	// timeout.tv_usec = 
	// 	res = select(maxfd + 1, &readfds, NULL, NULL, NULL);
	// 	if (res == -1) 
	// 	{
	// 		std::cout << "SELECT ERROR: " << strerror(errno) << std::endl;
	// 	}
	// 	if (FD_ISSET(socket_fd, &readfds))
	// 		server_accept_client();
	// 	for (int i = 0; i < FDs.size(); i++)
	// 	{
	// 		if (FD_ISSET(FDs[i], &readfds)) 
	// 		{
	// 			session_do_read();
	// 		}
	// 		if (FDs[i] > maxfd) 
	// 			maxfd = FDs[i];
	// 	}

	// 	exit(0);
	// }

	// struct sockaddr_in new_addr;
	// socklen_t new_addrlen = sizeof(sockaddr_in);
	// int new_fd;

	// /*		ACCEPT CONNECTION				*/
	// if ((new_fd = accept(socket_fd, (struct sockaddr*)&new_addr, &new_addrlen)) < 0) {
	// 	std::cout << "ACCEPT ERROR: " << strerror(errno) << std::endl;
	// }
	// else {
	// 	std::cout << "\n----CONNECTION ACCEPTED----\n";
	// /* 		the number of bytes that the call wrote to struct new_addr and new FD	*/
	// 	std::cout << "The number of bytes that the call wrote: " << new_addrlen << std::endl; 
	// 	std::cout << "FD of the client's socket: " << new_fd << std::endl;
	// }

	/*		SET NON_BLOCK TO SERVER			*/
	// int flags = fcntl(socket_fd, F_GETFL);
	// fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK);

	/*		SET NON_BLOCK TO CLIENT			*/
	// int flags = fcntl(new_fd, F_GETFL);
	// fcntl(new_fd, F_SETFL, flags | O_NONBLOCK);

	// char buffer[100];
	// int buf;
	
	/*		SEND TO CLIENT					*/
	// if (send(new_fd, buffer, strlen(buffer), MSG_SEND) < 0)
	// 	std::cout << "SEND ERROR: " << strerror(errno) << std::endl;
	
	/*		RECEIVE FROM CLIENT				*/

	// std::string stroka;
	// while ((buf = recv(new_fd, buffer, 99, MSG_WAITALL)) > 0) {
	// 	buffer[buf] = '\0';
	// 	stroka += buffer;
	// }
	// std::cout << stroka << std::endl;
	// std::cout << std::endl;
	// if (buf < 0)
	// 	std::cout << "RECV ERROR: " << strerror(errno) << std::endl;

	/*		SEND TO CLIENT					*/
	// if (send(new_fd, buffer, strlen(buffer), MSG_SEND) < 0)
	// 	std::cout << "SEND ERROR: " << strerror(errno) << std::endl;

	// close(socket_fd);
}