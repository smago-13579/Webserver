/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smago <smago@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/28 15:14:05 by smago             #+#    #+#             */
/*   Updated: 2021/06/03 17:16:44 by smago            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"

int main()
{
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd < 0) {
		std::cout << "Can't create socket\n";
	}
	else {
		std::cout << "\n----SOCKET CREATED----\n";
		std::cout << "Socket fd: " << socket_fd << std::endl;
	}
	
	/*		CREATE SOCKET ADDRESS			*/
	struct sockaddr_in addr;

	/*		CREATE IP AND PORT				*/
	addr.sin_family = AF_INET;
	addr.sin_port = htons(7654);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	/*		CONNECT TO WEB_SERVER			*/
	if (connect(socket_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)	{
		std::cout << "CONNECT ERROR: " << strerror(errno) << std::endl;
	}
	char buffer[100];
	int buf;

	/*		SET NON_BLOCK TO CLIENT			*/
	// int flags = fcntl(socket_fd, F_GETFL);
	// fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK);
	

	/*		RECEIVE FROM SERVER				*/
	// while ((buf = recv(socket_fd, buffer, 99, 0)) > 0) {
	// 	buffer[buf] = '\0';
	// 	std::cout << buffer;
	// }
	// std::cout << std::endl;
	// if (buf < 0)
	// 	std::cout << "RECV ERROR: " << strerror(errno) << std::endl;
	// std::cout << std::endl;
	
	std::string str;
	for (; ;) {
		std::getline(std::cin, str);
		if (str == "exit") {
			exit(0);
		}
		send(socket_fd, str.c_str(), str.length(), 0);

		/* 			RECEIVE FROM BUFFER				*/
		// buf = recv(socket_fd, buffer, 99, 0);
		// buffer[buf] = '\0';
		// std::cout << buffer << std::endl;
	}

	/*		SEND TO SERVER					*/
	// if (send(socket_fd, "Another message", strlen("Another message"), MSG_SEND) < 0)
	// 	std::cout << "SEND ERROR: " << strerror(errno) << std::endl;
	// std::cout << 

	close(socket_fd);

	return 0;
}
