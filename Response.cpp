/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngonzo <ngonzo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/11 17:31:33 by smago             #+#    #+#             */
/*   Updated: 2021/06/18 15:52:19 by ngonzo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "ngonzo/cgi_handler.hpp"

Response::Response() {}


Response::Response(const Request& req, Settings set)
{
	this->req = req;
	this->response_done	= 0;
	this->settings = &set;
	this->error_500 = "<!DOCTYPE html><html><body><h1 style=\"font-size:300%;\">Error 500</h1><h2 style=\"font-size:160%;\">Internal Server Error</h2></body></html>";

    loc_iter it = find_location();
    query_string.clear();
    std::cout << "! it->exec - " << it->exec << std::endl;
    std::cout << "! req.resource - " << this->req.resource << std::endl;
    if(it->exec != "")
    {
         if(this->req.resource.find("?") != std::string::npos)
        {
            int    ind = this->req.resource.find("?");
            query_string = this->req.resource.substr(ind + 1);
            ind = this->req.resource.size() - query_string.size();
            this->req.resource = this->req.resource.substr(0, ind - 1);
        }
    }
    std::cout << "! req.resource - " << this->req.resource << std::endl;
    std::cout << "! query_string - " << query_string << std::endl;

	find_method();
}

Response::~Response() {}

Response&	Response::operator=(const Response& tmp)
{
	req = tmp.req;
	settings = tmp.settings;
	answer = tmp.answer;
	response_done = tmp.response_done;
	error_500 = tmp.error_500;

	return (*this);
}

/******************/
/*                */
/*   Error pages  */
/*                */
/******************/
int			Response::error_page(int i)
{
	std::stringstream	response, body, file;
	int fd, res;
	char buff[100000];

	file << settings->error_page << "/" << "error_" \
	<< itoa(i) << ".html";
	std::cout << "FILE: " << file.str() << std::endl;

	if ((fd = open(file.str().c_str(), O_RDONLY)) < 0) {
		std::cout << "CAN'T OPEN FILE: " << file.str() << std::endl;
		return (-1);
	}
	while ((res = read(fd, buff, 99999)) > 0) {
		buff[res] = '\0';
		body << buff;
	}
	body << "\r\n\r\n";
	close(fd);
	if (res < 0) {
		std::cout << "CAN'T READ FILE: " << file.str() << std::endl;
		return (-1);
	}

	response << req.version << " " << itoa(i) << status_codes(i)
	<< "Version: " << req.version << "\r\n"
	<< get_headers(file.str());
	response << "Content-Length: " << body.str().length()
	<< "\r\n\r\n" << body.str();
	this->answer = response.str();
	
	this->response_done	= 1;
	
	return (0);
}

std::string			Response::status_codes(int i)
{
	std::string str;

	if (i == 400)
		str = " Bad Request\r\n";
	else if (i == 403)
		str = " Forbidden\r\n";
	else if (i == 404)
		str = " Not Found\r\n";
	else if (i == 500)
		str = " Internal Server Error\r\n";

	return (str);
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
	DIR* dir;

	file << get_path(loc);
	std::cout << "FILE: " << file.str() << std::endl;

	if (req.type == "GET" && (dir = opendir(file.str().c_str())) != NULL) {
		loc_iter it = find_location();
		closedir(dir);
		if (it->autoindex == ON) {
			std::string str = file.str();
			Autoindex autoi(str);
			body << autoi.getBody() << "\r\n\r\n";
		}
	}
	else if (req.type != "DELETE")
	{
		if ((fd = open(file.str().c_str(), O_RDONLY)) < 0) {
			std::cout << "CAN'T OPEN FILE: " << file.str() << std::endl;
			if (errno == EACCES)
				error_page(403);
			else if (errno == ENOENT)
				error_page(404);
			return (-1);
		}
		if (get_format(file.str()) == TEXT)
		{
			if(loc.exec != "")                             // ngonzo
				body << res_body;                          //
			else {                                         // ngonzo
				while ((res = read(fd, buff, 99999)) > 0) {
					buff[res] = '\0';
					body << buff;
				}
			}                                              // ngonzo
			body << "\r\n\r\n";
			close(fd);
			if (res < 0) {
				std::cout << "CAN'T READ FILE\n";
				return (-1);
			}
		}
		else
		{
			close(fd);
			image.open(file.str(), std::ifstream::binary);
			// if (!image.is_open()) {
			// 	std::cout << "CAN'T OPEN FILE: " << file.str() << std::endl;
			// 	return (-1);
			// }
			body << image.rdbuf() << "\r\n\r\n";
			image.clear();
			image.close();
		}
	}

	response << req.version << " 200 OK\r\n"
	<< "Version: " << req.version << "\r\n"
	<< get_headers(file.str());
    if (req.type != "DELETE")
	{
        response << "Content-Length: " << body.str().length()
		<< "\r\n\r\n" << body.str();
	}
	this->answer = response.str();
	
	this->response_done	= 1;
	
	return (0);
}

Response::loc_iter	Response::find_location()
{
	int i = 0;
	int comp;
	loc_iter itn = settings->locations.end();
	
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
	else if (req.type == "PUT")
		this->method_PUT();
	else if (req.type == "POST")
		this->method_POST(it);
}

int					Response::get_format(std::string str)
{
	if (str.find(".png") != std::string::npos || \
	str.find(".PNG") != std::string::npos || \
	str.find(".ico") != std::string::npos || \
	str.find(".jpeg") != std::string::npos || \
	str.find(".jpg") != std::string::npos)
	{
		std::cout << "IMAGE\n";
		return (IMAGE);
	}
	else if (str.find(".ttf") != std::string::npos ||
			str.find(".otf") != std::string::npos)
		return (FONT);
	return (TEXT);
}

std::string			Response::get_headers(std::string str)
{
	std::stringstream headers;
	time_t raw;
	time(&raw);
	
	int i = str.rfind(".") + 1;
	content_type = str.substr(i, str.size() - i);
	loc_iter it = find_location();                  // ngozno
	if(content_type == "py" and it->exec != "")     //
		content_type = "html";                      // ngozno
	std::cout << content_type << std::endl;
	if (str.rfind(".") == str.npos)
		content_type = "text/html";
	else if (get_format(str) == TEXT)
		content_type = "text/" + content_type;
	else if (get_format(str) == IMAGE)
		content_type = "image/" + content_type;
	else 
		content_type = "font/" + content_type;
	headers << "Server: DreamTeam/1.0.1 (School 21)\r\n"
	<< "Date: " << ctime(&raw)
	<< "Content-Type: " << content_type << "\r\n";

	return (headers.str());
}

std::string			Response::get_path(const Location& loc)
{
	std::stringstream str;
	loc_iter it = find_location();
	int pos = it->location.size();
	int len = req.resource.size();

	if (req.resource == loc.location && req.type != "DELETE")
		str << loc.root << "/" << loc.index;
	else
		str << loc.root << "/"
		<< req.resource.substr(pos, len - pos);
	return str.str();
}

std::string			Response::get_response() 
{
	if (response_done)
		return (this->answer);
	return ("");
}

void			Response::erase_answer(int i)
{
	this->answer.erase(0, i);
}

int			Response::method_GET()
{
	int i = 0;
	int comp;
	loc_iter it;

	it = find_location();
	if (it != settings->locations.end() && check_method(it->methods, GET) == 1) {
		if(it->exec != "")       // ngonzo
			method_POST(it);     //
		else                     // ngonzo
			create_response(*it);
		return (0);
	}
	else
	{
		std::cout << "\nBAD REQUEST\n"; // Bad request
		if (error_page(400) == 0)
			return (0);
	}
	return (-1);
}

int			Response::method_DELETE()
{
	//пока сможем удалять только из определенной папки
	loc_iter it;
	std::string file;

	it = find_location();
	if (check_method(it->methods, DELETE) == 1 && it->location.find("/images_for_delete/") != it->location.npos)
	{
		file = get_path(*it);
		if (remove(file.c_str()) != 0)
		{
			std::cout << "\nFILE NOT FOUND\n"; // Bad request
			if (error_page(404) == 0)
				return (0);
		}
		create_response(*it);
		return (0);
	}
	else
	{
		std::cout << "\nPERMISSION DENIED\n";
		if (error_page(403) == 0)
			return (0);
	}
	return (-1);
}

int			Response::method_PUT()
{
	return 0;
}

int			Response::method_POST(loc_iter &it)
{
	// if(req.body.size() > it->max_body)
	// {
	// 	req.body.clear();
	// 	// req.status_code_int_val = 413;
	// 	// req.reason_phrase = "Payload Too Large";
	// 	// req.headers.add_header("Content-Length", "0");
	// }
	// else if(req.body.size() == it->max_body or req.body.empty()) // and req.get_query_string().empty() )
	// {
	// 	req.body.clear(); // r.body = req.get_body();
	// 	// req.status_code_int_val = 200;
	// 	// req.reason_phrase = "OK";
	// 	// req.headers.add_header("Content-Length", std::to_string(req.body.size()));
	// }
	// else
	// {
//		std::cout << "!!! POST - ";                                        // for test
        std::vector<std::string>    env = cgi_env(it);
		cgi_handler cgi(env, it->root);
		cgi.req_body_to_fd(req.body);
		bool check = cgi.execute();
//		std::cout << "get_filename: " << cgi.get_filename() << std::endl; // for test
        // std::cout << "get_status_code: " << cgi.get_status_code() << std::endl;				// for test
        // std::cout << "get_str_content_type: " << cgi.get_str_content_type() << std::endl;	// for test
        // std::cout << "get_str_status_code: " << cgi.get_str_status_code() << std::endl;		// for test
        // std::cout << "get_response_body: " << cgi.get_response_body() << std::endl;			// for test
		if (check == true)
		{
            res_body = cgi.get_response_body();
            create_response(*it);
//            req.status_code_int_val = cgi.get_status_code();
//            req.reason_phrase = cgi.get_str_status_code();
//            req.headers.add_header("Content-Type", cgi.get_str_content_type());
//            req.headers.add_header("Content-Length", std::to_string(cgi.get_response_body().size()));
			return (0);
		}
		else
		{
			return (-1);// ERROR
		}
//	 }
	return (-1);// ERROR
}

std::vector<std::string>		Response::cgi_env(loc_iter &it)
{
//    int             ind;
//    std::string     query_string = "";
//    if(it->exec != "" and (ind = req.resource.find(it->exec)) != std::string::npos)
//    {
//            ind += it->exec.size();
//            query_string = req.resource.substr(ind);
//            ind = req.resource.size() - query_string.size();
//            req.resource = req.resource.substr(0, ind);
//    }
//    std::cout << "! req.resource - " << req.resource << std::endl;
//    std::cout << "! tmp_query_string - " << query_string << std::endl;

    std::vector<std::string>	tmp;
	tmp.push_back("AUTH_TYPE=anonymous");
	tmp.push_back("CONTENT_LENGTH=" + itoa(req.body.size()));
	tmp.push_back("CONTENT_TYPE=" + content_type);
	tmp.push_back("GATEWAY_INTERFACE=CGI/1.1");
	tmp.push_back("PATH_INFO=" + req.resource);
	tmp.push_back("PATH_TRANSLATED=" + it->root + req.resource);
	tmp.push_back("QUERY_STRING=" + query_string);
	tmp.push_back("REMOTE_ADDR=" + settings->ip);
	tmp.push_back("REMOTE_IDENT=." + req.headers["Host"]);
	tmp.push_back("REMOTE_USER=");
	tmp.push_back("REQUEST_METHOD=" + req.type);
	tmp.push_back("REQUEST_URI=" + req.resource);
	tmp.push_back("SCRIPT_NAME=" + it->exec);
	tmp.push_back("SERVER_NAME=" + settings->server_name);
	tmp.push_back("SERVER_PORT=" + itoa(settings->port));
	tmp.push_back("SERVER_PROTOCOL=" + req.version);
	tmp.push_back("SERVER_SOFTWARE=webserver");
	std::map<std::string, std::string>::iterator	begin = req.headers.begin(), end = req.headers.end();
	for (; begin != end; ++begin)
		tmp.push_back("HTTP_" + begin->first + "=" + begin->second);
//	// print env
//	for (std::vector<std::string>::iterator	begin = tmp.begin(), end = tmp.end(); begin != end; ++begin)
//		std::cout << "! " << *begin << std::endl;
//	// print env end
	return tmp;
}
