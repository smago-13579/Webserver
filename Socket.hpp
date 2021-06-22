/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smago <smago@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/10 15:20:44 by smago             #+#    #+#             */
/*   Updated: 2021/06/22 20:14:55 by smago            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

# include "webserv.hpp"
# include "Request.hpp"
# include "Response.hpp"
# include "Config.hpp"

# define GET 0
# define POST 1
# define DELETE 2

# define ON 1
# define OFF 0

class Socket
{
public:
	typedef std::vector<Config::Location>::iterator 	config_iter;

	Socket(Config::Server& server);
	~Socket();

	int		accept_client();
	int		create();
	void	close_socket(int fd);
	void	erase_request(int fd);
	int 	getFD();
	int 	socket_read(int fd);
	int 	socket_write(int fd);

	Settings settings;
	
private:
	std::map<int, Response> resp;
	std::map<int, Request> req;
	struct 	sockaddr_in	addr;
	int 	socket_fd;

};


#endif