/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smago <smago@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/11 17:31:33 by smago             #+#    #+#             */
/*   Updated: 2021/06/16 12:45:02 by smago            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response() {}


Response::Response(const Request& req, Settings set)
{
	this->req = req;
	this->response_done	= 0;
	this->settings = &set;
	content_type = this->req.headers["Accept"];
	find_method();
}

Response::~Response() {}

Response&	Response::operator=(const Response& tmp)
{
	req = tmp.req;
	settings = tmp.settings;
	answer = tmp.answer;
	response_done = tmp.response_done;

	return (*this);
}

int			Response::compare_prefix(std::string loc, std::string res)
{
	int i = 0;

	for ( ; loc[i] && res[i]; i++)
	{
		if (loc[i] != res[i])
			return (0);
	}
	if (loc[i] == '\0')
		return (i);
	return (0);
}

int			Response::check_method(std::vector<size_t>& methods, size_t cmd)
{
	if (methods.size() == 0)
		return (1);
	for (std::vector<size_t>::iterator it = methods.begin(); it != methods.end(); it++)
	{
		if (cmd == *it)
			return (1);
	}
	return (0);
}

int			Response::create_response(const Location& loc)
{
	std::stringstream	response, body, file;
	std::ifstream 		image;
	
	int fd, res;
	char buff[100000];

	if (req.resource == loc.location)
		file << loc.root << "/" << loc.index;
	else 
		file << loc.root << req.resource;
	std::cout << "FILE: " << file.str() << std::endl;
	
	if (file.str().find(".png") == std::string::npos && \
		file.str().find(".PNG") == std::string::npos && \
		file.str().find(".ico") == std::string::npos && \
		file.str().find(".jpeg") == std::string::npos && 
		file.str().find(".py") == std::string::npos)
	{
		if ((fd = open(file.str().c_str(), O_RDONLY)) < 0) {
			std::cout << "CAN'T OPEN FILE: " << file.str() << std::endl;
			return (-1);
		}
		while ((res = read(fd, buff, 99999)) > 0) {
			buff[res] = '\0';
			body << buff;
		}
		body << "\r\n\r\n";
		if (res < 0) {
			std::cout << "CAN'T READ FILE\n";		// correct to bad request
			return (-1);
		}
	}
	else 
	{
		image.open(file.str(), std::ifstream::binary);
		if (image.is_open())
			body << image.rdbuf() << "\r\n\r\n";
	}
	// else {
	// 	body << buff << "\r\n\r\n";
	// }
	response << req.version << " 200 OK\r\n"
	<< "Version: " << req.version << "\r\n"
	<< get_headers();
	response << "Content-Length: " << body.str().length()
	<< "\r\n\r\n" << body.str();
	this->answer = response.str();
	
	this->response_done	= 1;

	body.clear();
	image.clear();
	image.close();
	
	return (0);
}

Response::loc_iter	Response::find_location()
{
	int i = 0;
	int comp;
	loc_iter itn;
	
	for (loc_iter it = settings->locations.begin(); it != settings->locations.end(); it++)
	{
		if (it->location == req.resource)
			return (it);
	}
	for (loc_iter it = settings->locations.begin(); it != settings->locations.end(); it++)
	{
		comp = compare_prefix(it->location, req.resource);
		if (comp > i) 
		{
			itn = it;
			i = comp;
		}
	}
	return (itn);
}

void		Response::find_method()
{
	loc_iter it;

	it = find_location();
	if (req.type == "GET")
		this->method_GET();
	else if (req.type == "DELETE")
		this->method_DELETE();
	// else if (req.type == "POST")
	// {
		std::vector<std::string> vec;
		vec.push_back("AUTH_TYPE=anonymous");
		vec.push_back("CONTENT_LENGTH=" + itoa(req.body.size()));
		vec.push_back("CONTENT_TYPE=" + content_type);
		vec.push_back("GATEWAY_INTERFACE=CGI/1.1");
		vec.push_back("PATH_INFO=" + req.resource);
		vec.push_back("PATH_TRANSLATED=" + it->root + req.resource);
		vec.push_back("QUERY_STRING=");
		vec.push_back("REMOTE_ADDR=" + settings->ip);
		vec.push_back("REMOTE_IDENT=." + req.headers["Host"]);
		vec.push_back("REMOTE_USER=");
		vec.push_back("REQUEST_METHOD=" + req.type);
		vec.push_back("REQUEST_URI=" + req.resource);
		vec.push_back("SCRIPT_NAME=");		//	
		vec.push_back("SERVER_NAME=" + settings->server_name);
		vec.push_back("SERVER_PORT=" + itoa(settings->port));
		vec.push_back("SERVER_PROTOCOL=" + req.version);
		vec.push_back("SERVER_SOFTWARE=webserver");

		std::map<std::string, std::string>::iterator begin = req.headers.begin();
		for (; begin != req.headers.end(); begin++) {
			vec.push_back("HTTP_" + begin->first + "=" + begin->second);
		}
		// for (std::vector<std::string>::iterator begin = vec.begin(); begin != vec.end(); begin++)
		// {
		// 	std::cout << *begin << std::endl;
		// }
		
	// }
	/*  		ADD ANOTHER METHODS				*/
}

std::string			Response::get_response() 
{
	if (response_done)
		return (this->answer); 
	return ("");
}

void				Response::erase_answer(int i)
{
	this->answer.erase(0, i);
}

std::string			Response::get_headers()
{
	std::stringstream headers;
	time_t raw;
	time(&raw);
	
	content_type = req.headers["Accept"];
	
	// if (content_type.compare("text/html") == 0) 
	content_type = content_type.substr(0, content_type.find(","));
	content_type += ";";

	headers << "Server: DreamTeam/1.0.1 (School 21)\r\n"
	<< "Date: " << ctime(&raw)
	<< "Content-Type: " << content_type << "\r\n";

	return (headers.str());
}

int			Response::method_GET()
{
	int i = 0;
	int comp;
	loc_iter it;

	it = find_location();
	if (check_method(it->methods, GET) == 1) {
		create_response(*it);
		return (0);
	}
	else {
		std::cout << "\nCan't use get method\n";
		return (-1);
	}
	return (-1);
}

int			Response::method_DELETE()
{
	//пока сможем удалять только из определенной папки
	loc_iter it;

	it = find_location();
	if (check_method(it->methods, DELETE) == 1) {
		create_response(*it);
		return (0);
	}
	else {
		std::cout << "\nCan't use get method\n";
		return (-1);
	}
	
	

}
