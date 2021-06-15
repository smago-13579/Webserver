/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbatwoma <kbatwoma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/20 16:14:24 by monie             #+#    #+#             */
/*   Updated: 2021/06/15 10:59:36 by kbatwoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "webserv.hpp"
# include <sstream>

# define OK 0
# define WAITING 1 
# define ERROR 2
# define NEXT_STR "\r\n"
# define END_OF_HEADERS "\r\n\r\n"
# define END_OF_CHUNKED_BODY "\r\n0\r\n"

class Request {
	public:
		Request();
		Request(const Request&);
		~Request();
		Request	&operator=(const Request&);
		
		std::string 						buf;
		std::string							type;
		std::string							resource;
		std::string							version;
		std::map<std::string, std::string>	headers;
		std::string 						body;

		size_t	_request_done;

		void	processRequest(std::string &);
		void 	see_request();

	private:
		size_t	_status_st_line;
		size_t	_status_headers;
		int		_body_size;
		
		void	filling_start_line();
		void	filling_headers();
		void	filling_chunked_body();

		void	check_request();

		bool	find_body();
		int		hex_to_int_conv(std::string &);
};

#endif