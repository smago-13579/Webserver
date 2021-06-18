#include "cgi_handler.hpp"

// default constructor
cgi_handler::cgi_handler()
{ }

// test constructor
cgi_handler::cgi_handler(str filename)
	{ _filename = filename; }

// env constructor
cgi_handler::cgi_handler(std::vector<std::string> env, std::string & root)
{
	_parse_env(env);
	_root = root;
    _exec = env[12].substr(12);
    _filename = _root + "/" + _exec;
//    std::cout << "! _filename - " << _filename << std::endl;
	_response_body = "EMPTY";
	_status_code = -1;
	_str_status_code = "EMPTY";
	_str_content_type = "EMPTY";
}

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

char *		cgi_handler::_string_to_char(std::string str)
{
	const char *	tmp = str.c_str();
	char *			ret = new char [str.size() + 1];
	int i = 0;
	for(; tmp[i] != '\0'; ++i)
		ret[i] = tmp[i];
	ret[i] = '\0';
	return ret;
}

void		cgi_handler::_parse_env(std::vector<std::string> env)
{
	if(env.empty())
	{
		_env = NULL;
		return ;
	}
	//  // print env
	//  std::cout << "!!! - print input env" << std::endl;
	//  for (std::vector<std::string>::iterator	begin = env.begin(), end = env.end(); begin != end; ++begin)
	//  	std::cout << "! " << *begin << std::endl;
	//  // print env end
	_env = new char * [env.size() + 1];
	std::vector<std::string>::iterator	begin = env.begin(), end = env.end();
	int									i = 0;
	for (; begin != end; ++begin, ++i)\
		_env[i] = _string_to_char(*begin);
	_env[i] = NULL;
//	  // print new _env
//	  std::cout << "@@@ - print new _env" << std::endl;
//	  for (int i = 0; _env[i] != NULL; ++i)
//	  	std::cout << "@ " << _env[i] << std::endl;
//	  // print new _env end
}

void		cgi_handler::_free_env()
{
	if(_env == NULL)
		return ;
	int	i = 0;
	for(; _env[i] != NULL; ++i)
		delete _env[i];
	delete _env;
	_env = NULL;
}

void		cgi_handler::_parse_cgi()
{
	size_t		end_of_headers, i, j;
	std::string	header;

	end_of_headers = _response_body.find("\r\n\r\n");

//	header = _response_body.substr(0, end_of_headers);
//	i = header.find("Status: ") + 8;
//	if(header[i] >= '0' and header[i] <= '9')
//	{
//		for(; header[i] >= '0' and header[i] <= '9'; ++i)
//			_status_code = _status_code * 10 + (header[i] - '0');
//		++i;
//		j = header.find("\r\n", i) - i;
//		_str_status_code = header.substr(i, j);
//		_str_content_type = header.substr(j + i + 2, header.size() - 2);
//		_response_body.erase(0, end_of_headers + 4);
//	}

    _response_body.erase(0, end_of_headers + 4);
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
    if(_exec == "cgi_tester")
        return execute_tester();
    return execute_pipe();
}

bool		cgi_handler::execute_pipe()
{
	int		fd_pipe[2], fd_save[2], tmp;
	char	buff[buffer_size];

	fd_save[0] = dup(0);	// для чтения
	fd_save[1] = dup(1);	// для записи
	pipe(fd_pipe);
	dup2(fd_pipe[0], 0);
	dup2(fd_pipe[1], 1);
	if (fork() == 0)
	{
		char **args;
		strcpy(args[0], _filename.c_str());
		if(execve(_filename.c_str(), args, _env) == -1)
		{
			_restore_fd_and_close(fd_pipe, fd_save);
			exit(127);
		}
	}
	wait(&tmp);
	if(WEXITSTATUS(tmp) == 127)
		return _restore_fd_and_close(fd_pipe, fd_save);
	_response_body.clear();
	for(tmp = buffer_size; tmp == buffer_size ; _response_body += std::string(buff, tmp))
		if((tmp = read(fd_pipe[0], buff, buffer_size)) == -1)
			return _restore_fd_and_close(fd_pipe, fd_save);
	_parse_cgi();
	_restore_fd_and_close(fd_pipe, fd_save);
	// _test_write_to_file();						// for test
	// std::cout << _response_body << std::endl;	// for test
	return true;
}

bool		cgi_handler::execute_tester()
{
    int		fd[2], fd_save[2], tmp;
    char	buff[buffer_size];

    std::ofstream   temp(_root + "/temp");
    temp.open(_root + "/temp");

    fd[0] = open(std::string(_root + "/_input_data").c_str(), O_RDONLY);
    fd[1] = open(std::string(_root + "/temp").c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666);

    fd_save[0] = dup(0);	// для чтения
    fd_save[1] = dup(1);	// для записи
    if (fork() == 0)
    {
        dup2(fd[0], 0);
        dup2(fd[1], 1);

		char **args;
		strcpy(args[0], _filename.c_str());
        if (execve(_filename.c_str(), args, _env) == -1)
        {
            _restore_fd_and_close(fd, fd_save);
            exit(127);
        }
    }
    wait(&tmp);
    if(WEXITSTATUS(tmp) == 127)
        return _restore_fd_and_close(fd, fd_save);
    _response_body.clear();
    for(tmp = buffer_size; tmp == buffer_size ; _response_body += std::string(buff, tmp))
        if((tmp = read(fd[0], buff, buffer_size)) == -1)
            return _restore_fd_and_close(fd, fd_save);
    _parse_cgi();
    _restore_fd_and_close(fd, fd_save);
    // _test_write_to_file();						// for test
    // std::cout << _response_body << std::endl;	// for test
    return true;
}

void           cgi_handler::req_body_to_fd(std::string & request_body)
{
    _root_req_body = _root + "/_input_data";
    std::ofstream   req_body(_root_req_body);
    req_body << request_body;
//    this->_req_body << req_body;
}
