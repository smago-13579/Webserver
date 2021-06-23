/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbatwoma <kbatwoma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/20 15:15:16 by monie             #+#    #+#             */
/*   Updated: 2021/06/23 12:27:57 by kbatwoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

/**********************/
/*                    */
/*   Coplien's form   */
/*                    */
/**********************/
Request::Request() : buf(std::string()), type(std::string()), resource(std::string()),
				version(std::string()), headers(std::map<std::string, std::string>()), 
				body(std::string()), _request_done(WAITING), _status_st_line(WAITING),
				_status_headers(WAITING), _body_size(0)
{}

Request::Request(const Request &a) : buf(a.buf), type(a.type), resource(a.resource),
				version(a.version), headers(a.headers), body(a.body),
				_request_done(a._request_done), _status_st_line(a._status_st_line),
				_status_headers(a._status_headers), _body_size(a._body_size)
{
}

Request::~Request()
{
}

Request	&Request::operator=(const Request &a)
{
	if (this != &a)
	{
		buf = a.buf;
		type = a.type;
		resource = a.resource;
		version = a.version;
		headers = a.headers;
		body = a.body;
		_request_done = a._request_done;
		_status_st_line = a._status_st_line;
		_status_headers = a._status_headers;
		_body_size = a._body_size;
	}
	return(*this);
}

/**************/
/*            */
/*   Parser   */
/*            */
/**************/
void 	Request::processRequest(std::string &str)
{
    size_t  pos_end = 0;
    
	buf += str;
	/********************/
	/*   request line   */
	/********************/
    if (_status_st_line == WAITING){
        if ((pos_end = buf.find(NEXT_STR)) != buf.npos)
        {
            _status_st_line = OK;
            filling_start_line();
            buf.erase(0, pos_end + 2);
        }}
	/***************/
	/*   headers   */
	/***************/
    if (_status_headers == WAITING && _status_st_line == OK && _request_done != ERROR){
        if ((pos_end = buf.find(END_OF_HEADERS)) != buf.npos)
        {
            _status_headers = OK;
            filling_headers();
            buf.erase(0, pos_end + 4);
        }}
	/************/
	/*   body   */
	/************/
    if (find_body() == true && _status_headers == OK && _status_st_line == OK && _request_done != ERROR){
		if (_body_size == 0 && (pos_end = buf.find(END_OF_CHUNKED_BODY)) != buf.npos)
		{
			filling_chunked_body();
			buf.clear();
			_body_size = 0;
		}
		else if (_body_size > 0 && buf.length() >= static_cast<size_t>(_body_size))
		{
			body = std::string(buf, 0, _body_size);
			buf.clear();
			_body_size = 0;
		}
		std::cout << buf << std::endl;
	}
	/*************/
	/*   check   */
	/*************/
	if (_status_st_line == OK && _status_headers == OK && _body_size == 0)
		check_request();
}

void    Request::filling_start_line()
{
    size_t  pos_space = 0;
    size_t  pos_begin = 0;
    std::string new_str(buf, 0, buf.find(NEXT_STR));
	if ((pos_space = new_str.find(' ', pos_begin)) != new_str.npos)
	{
		type = std::string(new_str, pos_begin, pos_space - pos_begin);
        pos_begin = pos_space + 1;
	}
	if ((pos_space = new_str.find(' ', pos_begin)) != new_str.npos)
	{
		resource = std::string(new_str, pos_begin, pos_space - pos_begin);
        pos_begin = pos_space + 1;
	}
	if (pos_begin != new_str.npos)
	{
		version = std::string(new_str, pos_begin, pos_space - pos_begin);
        pos_begin = pos_space + 1;
	}

    if (type.empty() || resource.empty() || version.empty() || version != "HTTP/1.1")
		_request_done = ERROR;
}

void    Request::filling_headers()
{
	size_t  pos_end = 0;
    size_t  pos_begin = 0;
	size_t	pos_points;
    
	std::string new_str(buf, 0, buf.find(END_OF_HEADERS));
	std::string	key_val_str;
	std::string key;
	std::string val;

    while ((pos_end = new_str.find(NEXT_STR, pos_begin)) != new_str.npos)
    {
		key_val_str = std::string(new_str, pos_begin, pos_end - pos_begin);
		if ((pos_points = key_val_str.find(": ")) == key_val_str.npos)
		{
			_request_done = ERROR;
			return ;
		}
		key = std::string(key_val_str, 0, pos_points);
		pos_points += 2;
		val = std::string(key_val_str, pos_points, key_val_str.npos - pos_points);
		headers[key] = val;
        pos_begin = pos_end + 2;
    }
	key_val_str = std::string(new_str, pos_begin, pos_end - pos_begin);
	if ((pos_points = key_val_str.find(": ")) == key_val_str.npos)
	{
		_request_done = ERROR;
		return ;
	}
	key = std::string(key_val_str, 0, pos_points);
	pos_points += 2;
	val = std::string(key_val_str, pos_points, key_val_str.npos - pos_points);
	headers[key] = val;
}

void	Request::filling_chunked_body()
{
	size_t	pos_begin = 0;
	size_t	pos_end = 0;
	int		size_of_body = 0;

	std::string new_str(buf, 0, buf.find(END_OF_CHUNKED_BODY));
	std::string	hex_size_str;

	while (!new_str.empty() && (pos_end = new_str.find(NEXT_STR, pos_begin)) != new_str.npos)
    {
		hex_size_str = std::string(new_str, pos_begin, pos_end - pos_begin);
		size_of_body = hex_to_int_conv(hex_size_str);
		_body_size += size_of_body;
		pos_end += 2;
		body += std::string(new_str, pos_end, size_of_body);
		new_str.erase(0, pos_end + size_of_body);
		if (new_str[0] == '\r' && new_str[1] == '\n')
			new_str.erase(0, 2);
    }
}

/***************/
/*             */
/*   Checker   */
/*             */
/***************/
void	Request::check_request()
{
	if (headers.find("Host") != headers.end())
		_request_done = OK;
	else
		_request_done = ERROR;
}

/*************/
/*           */
/*   Utils   */
/*           */
/*************/
int		Request::hex_to_int_conv(std::string &hex_size_str)
{
	int number;
	std::stringstream str;
	str << hex_size_str;
	str >> std::hex >> number;
	return (number);
}

bool	Request::find_body()
{
	if (headers.find("Content-Length") != headers.end())
	{
		_body_size = atoi((headers["Content-Length"]).c_str());
		if (_body_size < 0)
		{	
			_request_done = ERROR;
			_body_size = 0;
		}
		return (true);
	}
	if (headers.find("Transfer-Encoding: chunked") != headers.end())
		return (true);
	return (false);
}

/*********************/
/*                   */
/*   Visualization   */
/*                   */
/*********************/
void	Request::see_request() {
	std::cout << "------------------------------------------------------------------------" << std::endl;
	std::cout << "-------------------------------start line-------------------------------" << std::endl;
	std::cout << "Type:\t\t" << type << std::endl;
	std::cout << "Resource:\t" << resource << std::endl;
	std::cout << "Version:\t" << version << std::endl;
	std::cout << "-------------------------------headers----------------------------------" << std::endl;
	std::map<std::string, std::string>::iterator it;
	for (it = headers.begin(); it != headers.end(); it++) {
		if((it->first).length() < 8) 
			std::cout << "key\t" << it->first << "\t\t\tvalue\t\t" << it->second << std::endl;
		else if((it->first).length() < 15)
			std::cout << "key\t" << it->first << "\t\tvalue\t\t" << it->second << std::endl;
		else
			std::cout << "key\t" << it->first << "\tvalue\t\t" << it->second << std::endl;
	}
	std::cout << "-------------------------------body-------------------------------------" << std::endl;
	std::cout << body << std::endl;
	std::cout << "-------------------------------end--------------------------------------" << std::endl;
}
