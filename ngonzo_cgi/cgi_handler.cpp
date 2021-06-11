#include "cgi_handler.hpp"

// test constructor
cgi_handler::cgi_handler(str filename)
	{ _filename = filename; }

// default constructor
cgi_handler::cgi_handler(Request request, t_client_addr client_addr)
	{ _create_env(&request, &client_addr); _construct_filename(); }

// filename constructor
cgi_handler::cgi_handler(Request request, t_client_addr client_addr, str filename)
	{ _create_env(&request, &client_addr); set_filename(filename); }

// default destructor
cgi_handler::~cgi_handler()
	{ _free_env(); }

// copy constructor
cgi_handler::cgi_handler(const cgi_handler & copy)
	{ *this = copy; }

// assignment
cgi_handler &	cgi_handler::operator=(const cgi_handler & rhs)
{
	_free_env();
	_env = rhs._env;
	_filename = rhs._filename;
	_response_body = rhs._response_body;
	return *this;
}

// // // --- --- --- --- private methods --- --- --- --- // // //

char *		cgi_handler::_ft_strjoin(std::string str1, std::string str2 = "", std::string str3 = "", std::string str4 = "")
{
	const char *	tmp;
	char *			ret;
	std::string		new_str = "";

	new_str += str1;
	new_str += str2;
	new_str += str3;
	new_str += str4;
	tmp = new_str.c_str();

	int i = 0;
	for(; tmp[i] != '\0'; ++i)
		ret[i] = tmp[i];
	ret[i] = '\0';
	
	return ret;
}

void		cgi_handler::_create_env(Request * request, t_client_addr * client_addr)
{
	// _env = new char *[18 + request->get_headers().size()];
	_env = new char *[18 + 0];

	_env[0] = _ft_strjoin("AUTH_TYPE=","");
	_env[1] = _ft_strjoin("CONTENT_LENGTH=","");
	_env[2] = _ft_strjoin("CONTENT_TYPE=","");
	_env[3] = _ft_strjoin("GATEWAY_INTERFACE=CGI/1.1");
	_env[4] = _ft_strjoin("PATH_INFO=","");
	_env[5] = _ft_strjoin("PATH_TRANSLATED=","");
	_env[6] = _ft_strjoin("QUERY_STRING=","");
	_env[7] = _ft_strjoin("REMOTE_ADDR=","");
	_env[8] = _ft_strjoin("REMOTE_IDENT=","");
	_env[9] = _ft_strjoin("REMOTE_USER=","");
	_env[10] = _ft_strjoin("REQUEST_METHOD=","");
	_env[11] = _ft_strjoin("REQUEST_URI=","");
	_env[12] = _ft_strjoin("SCRIPT_NAME=","");
	_env[13] = _ft_strjoin("SERVER_NAME=","");
	_env[14] = _ft_strjoin("SERVER_PORT=","");
	_env[15] = _ft_strjoin("SERVER_PROTACOL=","");
	_env[16] = _ft_strjoin("SERVER_SOFTWARE=web_serv");

	int	i = 17;
	std::map<std::string, std::string>::const_iterator	begin, end;
	// begin = request->get_headers().cbegin();
	// end = request->get_headers().cend();
	begin = end;
	for(; begin != end; ++begin, ++i)
		_env[i] = _ft_strjoin("HTTP_", begin->first, "=", begin->second);
	_env[i] = NULL;
}

void		cgi_handler::_construct_filename()
{
	_filename = "r.pwd + '/' + r.filename";
}

void		cgi_handler::_free_env()
{
	if(!_env)
		return ;
	int	i = 0;
	for(; _env[i] != NULL; ++i)
		delete _env[i];
	delete _env;
}

void		cgi_handler::_parse_cgi()
{
	size_t		end_of_headers, i, j;
	std::string	header;

	end_of_headers = _response_body.find("\r\n\r\n");
	header = _response_body.substr(0, end_of_headers);
	i = header.find("Status: ") + 8;
	if(header[i] >= '0' and header[i] <= '9')
	{
		for(; header[i] >= '0' and header[i] <= '9'; ++i)
			_status_code = _status_code * 10 + (header[i] - '0');
		++i;
		j = header.find("\r\n", i) - i;
		_str_status_code = header.substr(i, j);
		_str_content_type = header.substr(j + i + 2, header.size() - 2);
		_response_body.erase(0, end_of_headers + 4);
	}
}

void		cgi_handler::_test_write_to_file()
{
	int	save_out = dup(1);
	int	file_fd = open("file.txt", O_RDWR | O_CREAT | O_APPEND, S_IWRITE | S_IREAD, 0755);
	if(file_fd == -1)
		std::cout << "error: file.txt not create" << std::endl;
	dup2(file_fd, 1);
	std::cout << "   ---" << std::endl;
	std::cout << _response_body;
	dup2(save_out, 1);
	close(file_fd);
	close(save_out);
}

bool		cgi_handler::_restore_fd_and_close(int pipe[2], int save[2])
{
	dup2(save[0], 0);
	dup2(save[1], 1);
	close(pipe[0]);
	close(pipe[1]);
	close(save[0]);
	close(save[1]);
	_free_env();
	return false;
}

// // // --- --- --- - setters and getters - --- --- --- // // //

std::string const		cgi_handler::get_filename() const
	{ return _filename; }

void					cgi_handler::set_filename(str filename)
{
	_response_body = "";
	if(filename[0] == '/' or (filename[0] == '.' and filename[1] == '/'))
		_filename = filename;
	else
		_construct_filename();
}

std::string const	cgi_handler::get_response_body() const
	{ return _response_body; }

int const			cgi_handler::get_status_code() const
	{ return _status_code; }

std::string const	cgi_handler::get_str_content_type() const
	{ return _str_status_code; }

std::string const	cgi_handler::get_str_status_code() const
	{ return _str_content_type; }

// // // --- --- --- --- --- methods --- --- --- --- --- // // //

bool		cgi_handler::execute()
{
	int		fd_pipe[2], fd_save[2], tmp;
	char	buff[buffer_size];

	fd_save[0] = dup(0);	// для чтения
	fd_save[1] = dup(1);	// для записи
	pipe(fd_pipe);
	dup2(fd_pipe[0], 0);
	dup2(fd_pipe[1], 1);
	if (fork() == 0)
		if(execve(_filename.c_str(), NULL, NULL) == -1) // 3 argument = _env
		{
			_restore_fd_and_close(fd_pipe, fd_save);
			exit(127);
		}
	wait(&tmp);
	if(WEXITSTATUS(tmp) == 127)
		return _restore_fd_and_close(fd_pipe, fd_save);
	for(tmp = buffer_size; tmp == buffer_size ; _response_body += std::string(buff, tmp))
		if((tmp = read(fd_pipe[0], buff, buffer_size)) == -1)
			return _restore_fd_and_close(fd_pipe, fd_save);
	_parse_cgi();
	_restore_fd_and_close(fd_pipe, fd_save);
	_test_write_to_file();						// for test
	std::cout << _response_body << std::endl;	// for test
	return true;
}