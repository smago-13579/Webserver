/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngonzo <ngonzo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/11 17:27:58 by smago             #+#    #+#             */
/*   Updated: 2021/06/12 19:48:12 by ngonzo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "webserv.hpp"

# define GET 0
# define POST 1
# define DELETE 2

# define ON 1
# define OFF 0

class Response
{
public:
	typedef std::vector<struct Socket::Location>::iterator loc_iter;

	Response();
	Response(struct Request req, struct Socket::Settings set);
	~Response();

	std::string			get_response();
	
private:
	Request *			req;
	Socket::Settings* 	settings;
	std::stringstream 	response;
	std::stringstream 	body;
	std::stringstream 	file;
	size_t				response_done;
	
	void			find_method();
	int 			get_method();
	void			get_method_post();		//ngonzo
	int				create_response(Socket::Location& loc);
	int				check_method(std::vector<size_t>& methods, size_t cmd);
};

# include "Request.hpp"
# include "Socket.hpp"

#endif