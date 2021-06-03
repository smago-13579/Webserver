/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smago <smago@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/02 12:40:33 by smago             #+#    #+#             */
/*   Updated: 2021/06/03 15:13:07 by smago            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "webserv.hpp"

class Server
{
public:
	Server();
	~Server();

	void 	server_run();
private:
	void 	accept_client();
	int 	server_read(int fd);

	struct 	sockaddr_in	addr;
	int 	socket_fd;
	std::vector<int> FDs; // modify to map
};



#endif
