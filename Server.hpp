/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smago <smago@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/02 12:40:33 by smago             #+#    #+#             */
/*   Updated: 2021/06/14 12:29:57 by smago            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "webserv.hpp"
# include "Socket.hpp"
# include "kbatwoma/config/Config.hpp"


class Server
{
	typedef std::map<int, class Socket>::iterator	serv_iter;
	typedef	std::map<int, class Socket*>::iterator	cls_iter;
	typedef std::vector<Config::Server>::iterator	conf_iter;

public:
	Server(std::string str);
	~Server();

	void 	server_run();

	class Abort_server : public std::exception
	{
		const char* what() const throw();
	};

private:
	fd_set	readfds, writefds;

	/*			Chained classes	and containers		*/
	class Config*							config;
	std::map<int, Socket>					servers;
	std::map<int, Socket*>					clients;
	
	/*			Do I need this?				
	std::vector<int> del_FD;		// delete FD
	*/
};



#endif
