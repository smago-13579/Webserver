/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngonzo <ngonzo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/11 17:27:58 by smago             #+#    #+#             */
/*   Updated: 2021/06/17 12:59:24 by ngonzo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "webserv.hpp"
# include "Request.hpp"
# include <algorithm>

# define GET 0
# define POST 1
# define DELETE 2

# define ON 1
# define OFF 0

# define TEXT 0
# define IMAGE 1

class Response
{
public:
	typedef std::vector<Location>::iterator	loc_iter;
	typedef std::vector<std::string>		array;

	Response();
	Response(const Request& req, Settings set);
	~Response();

	Response&			operator=(const Response& tmp);

	void				erase_answer(int i);
	std::string			get_response();
	
private:
	Request				req;
	Settings* 			settings;
	std::string			answer;
	std::string			content_type;
	size_t				response_done;

	int				error_page(int i);
	int				compare_prefix(std::string loc, std::string res);
	int				create_response(const Location& loc);
	int				check_method(std::vector<size_t>& methods, size_t cmd);
	loc_iter		find_location();
	void			find_method();
	int				get_format(std::string str);
	std::string		get_headers(std::string str);
	std::string		get_path(const Location& loc);
	int 			method_GET();
	int				method_DELETE();
	int				method_PUT();
	int				method_POST(loc_iter &it);
	array			cgi_env(loc_iter &it);
	std::string		status_codes(int i);

};

#endif