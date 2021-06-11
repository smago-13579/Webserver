/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smago <smago@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/11 17:31:33 by smago             #+#    #+#             */
/*   Updated: 2021/06/11 22:05:11 by smago            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response() {}


Response::Response(Request req, Socket::Settings set)
{
	this->req = &req;
	this->response_done	= 0;
	this->settings = &set;
	find_method();
}

Response::~Response() {}


int			Response::check_method(std::vector<size_t>& methods, size_t cmd)
{
	for (std::vector<size_t>::iterator it = methods.begin(); it != methods.end(); it++)
	{
		if (cmd == *it)
			return (1);
	}
	return (0);
}

int			Response::create_response(Socket::Location& loc)
{
	int fd, res;
	char buff[32648];

	if (req->resource == loc.location)
		file << loc.root << loc.index;
	else 
		file << loc.root << req->resource;
	
	if ((fd = open(file.str().c_str(), O_RDONLY)) < 0) {
		std::cout << "CAN'T OPEN FILE" << std::endl;
		return (-1);
	}

	res = read(fd, buff, 32647);
	buff[res] = '\0';
	
	body << buff << "0\r\n\r\n";
	response << req->version << " 200 OK\r\n"
	<<	"Version: " << req->version << "\r\n"
	<< "Content-Type: text/html; charset=\"UTF-8\"\r\n";
	response << "Content-Length: " << body.str().length()
	<< "\r\n\r\n" << body.str();
	
	this->response_done	= 1;
	
	return (0);
}

void		Response::find_method()
{
	if (req->type == "GET")
		this->get_method();
	// else if (req->type == "PUT")
}

int			Response::get_method()
{
	for (loc_iter it = settings->locations.begin(); it != settings->locations.end(); it++)
	{
		if (it->location == req->resource) {
			if (check_method(it->methods, GET) == 1) {
				create_response(*it);
				return (0);
			}
			else {
				std::cout << "\nCan't use get method\n";
				return (-1);
			}
		}
	}
	return (0);
}

std::string			Response::get_response() 
{
	if (response_done)
		return response.str(); 
	return ("");
}
