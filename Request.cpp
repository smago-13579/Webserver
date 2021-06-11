/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smago <smago@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/20 15:15:16 by monie             #+#    #+#             */
/*   Updated: 2021/06/11 22:19:53 by smago            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

size_t		Request::str_to_size_t(std::string str) {
	int ui = 0;
	int len = str.length();
	for(int i = 0; i < len; i++) {
		if(i != 0) {
			ui *= 10;
		}
		ui += str[i] - '0';
	}
	return ui;
}

size_t		Request::hex_to_size_t(std::string st)
{
    size_t i, k;
    unsigned int s = 0;
    for (i = 0; st[i] != '\0'; i++)
    {
        int c;
        switch (c = toupper(st[i]))
        {
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
            k = c - 'A' + 10;
            break;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '0':
            k = c - '0';
            break;
        }
        s = (s << 4) + k;
    }
    return s;
}

void	Request::request_init() 
{
	_f_sl_status = 0;
	_f_hd_status = 0;
	_f_bd_status = 0;
	_request_done = 0;
}

size_t		Request::find_end(std::string str, std::string end) 
{
	size_t len = str.find(end);

	if (len == std::string::npos)
		return 1;

	for (size_t i = 0; i < end.length(); i++)
	{
		if (str[len + i] != end[i])
			return 1;
	}
	return 0;
}

void	Request::filling_start_line(std::string& str) 
{
	size_t i = str.find("\r\n");
	size_t len = str.find_first_of(" \t\r\n");
	size_t start = 0;

	while (len < i)
	{
		if (type.length() == 0)
			type = str.substr(start, len - start);
		else if (resource.length() == 0)
			resource = str.substr(start, len - start);
		start = str.find_first_not_of(" \t\r\n", len);
		len = str.find_first_of(" \t\r\n", start);
	}
	version = str.substr(start, len - start);
	str.erase(0, i + 2);
	_f_sl_status = 1;
}

void	Request::filling_headers(std::string& str) 
{
	std::string key;
	std::string word;
	size_t i = str.find("\r\n\r\n");
	size_t start = 0;
	size_t len = str.find(":");
	
	while (len < i)
	{
		if (key.length() == 0)
		{
			key = str.substr(start, len - start);
			start = str.find_first_not_of(" \t\r\n", len + 1);
			len = str.find_first_of("\r\n", start);
		}
		else
		{
			word = str.substr(start, len - start);
			headers.insert(make_pair(key, word));
			key.clear();
			word.clear();
			start = str.find_first_not_of(" \t\r\n", len);
			len = str.find_first_of(": ", start);
		}
	}
	str.erase(0, i + 4);
	_f_hd_status = 1;
	if (str.empty())
		_request_done = 1;
}

void	Request::filling_body(std::string& str)
{
	body = str;
	str.clear();
	_request_done = 1;
}

void	Request::func_request(std::string str) 
{
	while (!str.empty())
	{
		if(_f_sl_status == 0)
			filling_start_line(str);
		else if (_f_hd_status == 0)
			filling_headers(str);
		else if (_f_bd_status == 0)
			filling_body(str);
	}
}

int		Request::check_request(std::string str)
{
	size_t len;

	if ((len = str.find("\r\n\r\n")) != std::string::npos) 
	{
		if (str.find("Content-Length: ") == std::string::npos)
		{
			if (str.find("Transfer-Encoding: chunked") != std::string::npos)
			{
				if (find_end(str, "0\r\n\r\n") == 0)
					return (0);
				else
					return (1);
			}
			else
				return (0);
		}
		
		std::string tmp = str.substr(str.find("Content-Length: ") + 16, 10);
		if (std::atoi(tmp.c_str()) + 4 + len <= str.length())
			return (0);
		else
			return (1);
	}

	return (1);
}

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

Request&	Request::operator=(std::string str)
{
	if (buf.length() != 0)
	{
		buf += str;
		str = buf;
	}

	if (check_request(str) == 0)
		func_request(str);
	else
		buf = str;
	
	// this->see_request();
	return (*this);
}

Request::Request(std::string str):
buf(""), type(""), version(""), resource(""), body("")
{
	this->request_init();

	if (check_request(str) == 0)
		func_request(str);
	else 
		buf = str;
	this->see_request();
}

Request&	Request::operator=(Request& tmp)
{
	if (this != &tmp)
	{
		this->buf = tmp.buf;
		this->type = tmp.type;
		this->resource = tmp.resource;
		this->version = tmp.version;
		this->body = tmp.body;
		this->_f_sl_status = tmp._f_sl_status;
		this->_f_hd_status = tmp._f_hd_status;
		this->_f_bd_status = tmp._f_bd_status;
		this->_request_done = tmp._request_done;
		this->headers = tmp.headers;
	}
	return (*this);
}


Request::Request(Request& tmp)
{
	*this = tmp;
}
