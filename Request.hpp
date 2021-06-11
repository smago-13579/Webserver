/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smago <smago@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/20 16:14:24 by monie             #+#    #+#             */
/*   Updated: 2021/06/11 22:18:58 by smago            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "webserv.hpp"

class Request {
public:
	Request(std::string str);
	Request(Request& tmp);
	
	Request&		operator=(std::string str);
	Request&		operator=(Request& tmp);
	
	std::string 	buf;
	std::string		type;
	std::string		resource;
	std::string		version;
	std::string 	body;
	size_t			_f_sl_status;
	size_t			_f_hd_status;
	size_t			_f_bd_status;
	size_t			_request_done;

	std::map<std::string, std::string> headers;

	int				check_request(std::string str);
	size_t 			find_end(std::string str, std::string end);
	void 			filling_start_line(std::string& str);
	void 			filling_headers(std::string& str);
	void 			filling_body(std::string& str);
	void 			request_init();
	void 			func_request(std::string str);
	void 			see_request();
	size_t			hex_to_size_t(std::string str);
	size_t 			str_to_size_t(std::string str);
};

#endif