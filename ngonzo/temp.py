#!/usr/local/bin/python3

print("Content-type: text/html")
print()
print("<h1>Hello world!</h1>")

if	str[0] == str[1] == str[2]\
	or str[3] == str[4] == str[5]\
	or str[6] == str[7] == str[8]\
	or str[0] == str[3] == str[6]\
	or str[1] == str[4] == str[7]\
	or str[2] == str[5] == str[8]\
	or str[0] == str[4] == str[8]\
	or str[2] == str[4] == str[6]:
	print("Winner is " , str[9])
else:
	print("Draw")
 
AUTH_TYPE=Anonymous
CONTENT_LENGTH=100000
CONTENT_TYPE=test/file
GATEWAY_INTERFACE=CGI/1.1
PATH_INFO=/directory
PATH_TRANSLATED=/Users/anatashi/goinfre/webServ/webServ/YoupiBanane/directory
QUERY_STRING=
REMOTE_ADDR=127.0.0.1
REMOTE_IDENT=.localhost:8080
REMOTE_USER=
REQUEST_METHOD=POST
REQUEST_URI=/directory
SCRIPT_NAME=cgi_tester
SERVER_NAME=for_tester
SERVER_PORT=8080
SERVER_PROTOCOL=HTTP/1.1
SERVER_SOFTWARE=webServ
HTTP_accept-encoding=gzip
HTTP_content-type=test/file
HTTP_host=localhost:8080
HTTP_transfer-encoding=chunked
HTTP_user-agent=go-http-client/1.1
HTTP_x-secret-header-for-test=1

void		cgi_handler::_create_env(Request * request)
{
	// _env = new char *[18 + request->get_headers().size()];
	_env = new char *[18 + 0];

	_env[0] = _ft_strjoin("AUTH_TYPE=","anonymous");	// по умолчанию анонимус
	_env[1] = _ft_strjoin("CONTENT_LENGTH=","");	// req.body.size(); - длина бади запроса, по умолчанию 0
	_env[2] = _ft_strjoin("CONTENT_TYPE=","");		// req.headers["Accept"] - Accept: text/html
	_env[3] = _ft_strjoin("GATEWAY_INTERFACE=CGI/1.1"); // 
	_env[4] = _ft_strjoin("PATH_INFO=","");			// req.resource
	_env[5] = _ft_strjoin("PATH_TRANSLATED=","");	// location.root + req.resource
	_env[6] = _ft_strjoin("QUERY_STRING=","");		// 
	_env[7] = _ft_strjoin("REMOTE_ADDR=","");		// settings->ip
	_env[8] = _ft_strjoin("REMOTE_IDENT=","");		// req.headers["User-Agent"] + "." + req.headers["Host"]
	_env[9] = _ft_strjoin("REMOTE_USER=","");		// req.headers["User-Agent"]
	_env[10] = _ft_strjoin("REQUEST_METHOD=","");	// req.type
	_env[11] = _ft_strjoin("REQUEST_URI=","");		// req.resource
	_env[12] = _ft_strjoin("SCRIPT_NAME=","");		//
	_env[13] = _ft_strjoin("SERVER_NAME=","");		// settings->server_name
	_env[14] = _ft_strjoin("SERVER_PORT=","");		// settings->port
	_env[15] = _ft_strjoin("SERVER_PROTACOL=","");	// req.version
	_env[16] = _ft_strjoin("SERVER_SOFTWARE=webserver");

	int	i = 17;
	std::map<std::string, std::string>::const_iterator	begin, end;
	// begin = request->get_headers().cbegin();
	// end = request->get_headers().cend();
	begin = end;
	for(; begin != end; ++begin, ++i)
		_env[i] = _ft_strjoin("HTTP_", begin->first, "=", begin->second);
	_env[i] = NULL;
}

server {
	listen		127.0.0.1:8000;
	server_name	for_tester;
	error_page	/Users/ngonzo/Desktop/projects/webserv/html/error_page;
	location / {
		methods		GET;
		index		index.html;
		root		/Users/ngonzo/Desktop/projects/webserv/html;
	}
	location /put_test {
		methods		PUT;
		index		index.html;
		root		/Users/ngonzo/Desktop/projects/webserv/html;
	}
	location /post_body {
		methods		POST;
		index		index.html;
		root		/Users/ngonzo/Desktop/projects/webserv/html;
		max_body	100;
		exec		cgi_tester;
	}
	location /directory {
		methods		GET, POST;
		index		youpi.bad_extension;
		root		/Users/ngonzo/Desktop/projects/webserv/html/YoupiBanane;
		exec		cgi_tester;
	}
}