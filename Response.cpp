/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngonzo <ngonzo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/11 17:31:33 by smago             #+#    #+#             */
/*   Updated: 2021/06/21 16:29:48 by ngonzo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "Cgi_handler.hpp"

Response::Response() {}

Response::Response(const Request& req, Settings set)
{
	this->req = req;
	this->response_done	= 0;
	this->settings = &set;
	
	if (settings->redirect.empty() && (it = find_location()) == settings->locations.end()) {
		std::cout << "\nBAD REQUEST\n";
		this->error_page(400);
			return ;
	}
	if (settings->redirect.empty())
		this->autoindex = it->autoindex;
	this->content_type = "";

	/*				ngonzo						*/
<<<<<<< HEAD
    query_string.clear();
    // std::cout << "! it->exec - " << it->exec << std::endl;
    // std::cout << "! req.resource - " << this->req.resource << std::endl;
    if(settings->redirect.empty() && it->exec != "")
    {
         if(this->req.resource.find("?") != std::string::npos)
        {
            int    ind = this->req.resource.find("?");
            query_string = this->req.resource.substr(ind + 1);
            ind = this->req.resource.size() - query_string.size();
            this->req.resource = this->req.resource.substr(0, ind - 1);
        }
    }
    // std::cout << "! req.resource - " << this->req.resource << std::endl;
    // std::cout << "! query_string - " << query_string << std::endl;
=======
	query_string.clear();
	if(it->exec != "")
	{
		int	ind = this->req.resource.find("?");
		if(ind != std::string::npos)
		{
			query_string = this->req.resource.substr(ind + 1);
			ind = this->req.resource.size() - query_string.size();
			this->req.resource = this->req.resource.substr(0, ind - 1);
		}
	}
>>>>>>> master
	/*				ngonzo						*/

	find_method();
}

Response::Response(int error, Settings set)
{
	this->response_done	= 0;
	this->settings = &set;
	this->error_page(error);
}

Response::~Response() {}

Response&	Response::operator=(const Response& tmp)
{
	req = tmp.req;
	settings = tmp.settings;
	answer = tmp.answer;
	response_done = tmp.response_done;
	autoindex = tmp.autoindex;
	content_type = tmp.content_type;
	it = tmp.it;

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
	get_format(file.str());

	if ((fd = open(file.str().c_str(), O_RDONLY)) < 0) {
		std::cout << "CAN'T OPEN FILE: " << file.str() << std::endl;
			body << ::error_page(i) << "\r\n\r\n";
	}
	else
	{
		while ((res = read(fd, buff, 99999)) > 0) {
			buff[res] = '\0';
			body << buff;
		}
		if (res < 0) {
			std::cout << "CAN'T READ FILE: " << file.str() << std::endl;
			body << ::error_500();
		}
		body << "\r\n\r\n";
		close(fd);
	}

	response << "HTTP/1.1 " << itoa(i) << status_codes(i);
	if (i == 405)
		response << "Allow: " << this->get_method();
	response << "Version: HTTP/1.1\r\n"
	<< get_headers(file.str())
	<< "Content-Length: " << body.str().length()
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
	else if (i == 405)
		str = " Method Not Allowed\r\n";
	else if (i == 413)
		str = " Payload Too Large\r\n";
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

std::string Response::get_method()
{
	int i = it->methods[0];
	std::string str = " \r\n";
	if (i == GET)
		str = "GET\r\n";
	else if (i == POST)
		str = "POST\r\n";
	else if (i == DELETE)
		str = "DELETE\r\n";
	else if (i == PUT)
		str = "PUT\r\n";
	return str;
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

int			Response::create_response(const Location& loc, std::string status)
{
	std::stringstream	response, body;
	std::ifstream 		image;
	std::string			file;
	int 				fd, res;
	char 				buff[100000];

	file = get_path(loc);
	std::cout << "FILE: " << file << std::endl;

	if (req.type == "GET" && format == DIRC && this->autoindex == ON) {
			Autoindex autoIn(file);
			body << autoIn.getBody() << "\r\n\r\n";
	}
	else if (req.type != "DELETE" && req.type != "PUT")
	{
		if ((fd = open(file.c_str(), O_RDONLY)) < 0) {
			std::cout << "CAN'T OPEN FILE: " << file << std::endl;
			if (errno == EACCES)
				return (error_page(403));
			else if (errno == ENOENT)
				return (error_page(404));
		}
		if (req.type == "POST" && loc.exec != "")
			body << res_body << "\r\n\r\n";
		else if (format == TEXT)
		{
			while ((res = read(fd, buff, 99999)) > 0) {
				buff[res] = '\0';
				body << buff;
			}
			body << "\r\n\r\n";
			if (res < 0) {
				std::cout << "CAN'T READ FILE\n";
				return (error_page(500));
			}
		}
		else
		{
			image.open(file, std::ifstream::binary);
			body << image.rdbuf() << "\r\n\r\n";
			image.clear();
			image.close();
		}
		close(fd);
	}

	response << req.version << status
	<< "Version: " << req.version << "\r\n"
	<< get_headers(file, req.type, req.resource);
	if (req.type != "DELETE" && req.type != "PUT")
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
		std::cout << "it->location: " << it->location << "\treq.resource: " << req.resource << std::endl;
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
	if (!(settings->redirect.empty()))
		this->redirect();
	else if (req.type == "GET")
		this->method_GET();
	else if (req.type == "DELETE")
		this->method_DELETE();
	else if (req.type == "PUT")
		this->method_PUT();
	else if (req.type == "POST")
		this->method_POST(it);
	else
		error_page(405);
}

int					Response::get_format(std::string str)
{
	DIR* dir;

	if ((dir = opendir(str.c_str())) != NULL) 
	{
		closedir(dir);
		format = DIRC;
	}
	else if (str.find(".png") != std::string::npos || \
		str.find(".PNG") != std::string::npos || \
		str.find(".ico") != std::string::npos || \
		str.find(".jpeg") != std::string::npos || \
		str.find(".jpg") != std::string::npos) 
	{
		format = IMAGE;
	}
	else if (str.find(".ttf") != std::string::npos || \
			str.find(".otf") != std::string::npos)
		format = FONT;
	else
		format = TEXT;
	
	return (format);
}

std::string			Response::get_headers(std::string str, std::string type, std::string location)
{
	std::stringstream headers;
	time_t raw;
	time(&raw);
	size_t i;
	
	if ((i = str.rfind(".")) != str.npos)
		content_type = str.substr(i + 1, str.size() - i + 1);
	if((content_type == "py" || content_type == "php") && it->exec != "")
		content_type = "html";
	if (format == DIRC)
		content_type = "text/html";
	else if (format == TEXT)
		content_type = "text/" + content_type;
	else if (format == IMAGE)
		content_type = "image/" + content_type;
	else if (format == FONT)
		content_type = "font/" + content_type;
	headers << "Server: DreamTeam/1.0.1 (School 21)\r\n"
	<< "Date: " << ctime(&raw);
	if (req.type != "PUT")
		headers << "Content-Type: " << content_type << "\r\n";
	else
		headers << "Content-Location: " << location << "\r\n";

	return (headers.str());
}

std::string			Response::get_path(const Location& loc)
{
	std::stringstream str;
	int pos = it->location.size();
	int len = req.resource.size();

	if (it->location[pos - 1] != '/')
		pos++;
	
	if (req.resource == loc.location && req.type != "DELETE") {
		if (this->autoindex == OFF && req.type != "POST")
			str << loc.root << "/" << loc.index;
		else
			str << loc.root;
	}
	else {
		str << loc.root << "/"
		<< req.resource.substr(pos, len - pos);
	}
	get_format(str.str());
	if (this->format == DIRC && this->autoindex == OFF && req.type != "POST")
		str << "/" << it->index;
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
	if (check_method(it->methods, GET) == 1) {
		if(it->exec.find(".") != std::string::npos)
		{
			req.type = "POST";
			method_POST(it);
		}
		else
			create_response(*it);
	}
	else
	{
		std::cout << "\nBAD REQUEST\n";
		error_page(405);
	}
	return (0);
}

int			Response::method_DELETE()
{
	std::string file;

	if (check_method(it->methods, DELETE) == 1 && it->location.find("/images_for_delete/") != it->location.npos)
	{
		file = get_path(*it);
		if (remove(file.c_str()) != 0)
		{
			std::cout << "\nFILE NOT FOUND\n";
			return (error_page(404));
		}
		create_response(*it);
	}
	else
	{
		std::cout << "\nPERMISSION DENIED\n";
		error_page(405);
	}
	return (0);
}

int			Response::method_PUT()
{
	std::string status = " 200 OK\r\n";

	std::string file_path = get_path(*it);
	if (check_method(it->methods, PUT) == 1 && file_path.find("/images_for_delete/") != file_path.npos)
	{
		std::ifstream exist_file(file_path);
		if (exist_file.is_open())
		{
			status = " 204 No Content\r\n";
			exist_file.close();
		}
		else
			status = " 201 Created\r\n";
		
		std::ofstream file;
		file.open(file_path);
		if (file.is_open())
		{
			file << req.body;
			file.close();
			create_response(*it, status);
		}
		else
		{
			std::cout << "\nFILE NOT OPENED\n";
			error_page(500);
		}
	}
	else
	{
		std::cout << "\nPERMISSION DENIED\n";
		error_page(405);
	}
	return (0);
}

int			Response::method_POST(loc_iter &it)
{
	if (check_method(it->methods, POST) != 1) {
		error_page(405);
	}
	else if(req.body.size() > it->max_body)
	{
		// std::cout << "\n! body.size > max_body\n";                    // for test
		error_page(413);
	}
	// else if(req.body.empty() and query_string.empty() && it->exec.find(".") == std::string::npos)
	// {
	// 	std::cout << "\n! body.size == max_body\n";                   // for test
	// 	create_response(*it);
	// }
	else
	{
		// std::cout << "\n! POST \n";                                        // for test
		std::vector<std::string> env = cgi_env(it);
		cgi_handler cgi(env, it->root);
		if(it->exec == "cgi_tester")
			cgi.req_body_to_fd(req.body);
		bool check = cgi.execute();
		if (check == true)
		{
			res_body = cgi.get_response_body();
			create_response(*it);
			return (0);
		}
		else
		{
			error_page(500);
			return -1;
		}
	}
	return 0;
}

int			Response::redirect()
{
	std::stringstream response;
	std::string status = " 301 Moved Permanently\r\n";

	response << req.version << status
	<< "Location: " << settings->redirect << "\r\n\r\n";
	this->answer = response.str();
	this->response_done	= 1;

	return (0);
}

std::vector<std::string>		Response::cgi_env(loc_iter &it)
{
	std::vector<std::string>	tmp;
	tmp.push_back("AUTH_TYPE=anonymous");
	tmp.push_back("CONTENT_LENGTH=" + itoa(req.body.size()));
	tmp.push_back("CONTENT_TYPE=" + content_type);
	tmp.push_back("GATEWAY_INTERFACE=CGI/1.1");
	tmp.push_back("PATH_INFO=" + req.resource);
	tmp.push_back("PATH_TRANSLATED=" + it->root);// + req.resource);
	tmp.push_back("QUERY_STRING=" + query_string);
	tmp.push_back("REMOTE_ADDR=" + settings->ip);
	tmp.push_back("REMOTE_IDENT=." + req.headers["Host"]);
	tmp.push_back("REMOTE_USER=");
	tmp.push_back("REQUEST_METHOD=GET");// + req.type);
	tmp.push_back("REQUEST_URI=" + req.resource);
	tmp.push_back("SCRIPT_NAME=" + it->exec);
	tmp.push_back("SERVER_NAME=" + settings->server_name);
	tmp.push_back("SERVER_PORT=" + itoa(settings->port));
	tmp.push_back("SERVER_PROTOCOL=" + req.version);
	tmp.push_back("SERVER_SOFTWARE=webserver");
	std::map<std::string, std::string>::iterator	begin = req.headers.begin(), end = req.headers.end();
	for (; begin != end; ++begin)
		tmp.push_back("HTTP_" + begin->first + "=" + begin->second);
	// // print env
	// for (std::vector<std::string>::iterator	begin = tmp.begin(), end = tmp.end(); begin != end; ++begin)
	// 	std::cout << "! " << *begin << std::endl;
	// // print env end
	return tmp;
}
