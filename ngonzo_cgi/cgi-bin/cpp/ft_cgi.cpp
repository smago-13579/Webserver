#include "ft_cgi.hpp"

// default constructor
ft_cgi::ft_cgi()
	{ _create_env(); _construct_filename(); }

// filename constructor
ft_cgi::ft_cgi(str filename)
	{ _create_env(); set_filename(filename); }

// default destructor
ft_cgi::~ft_cgi()
	{ _free_env(); }

// copy constructor
ft_cgi::ft_cgi(const ft_cgi & copy)
	{ *this = copy; }

// assignment
ft_cgi &	ft_cgi::operator=(const ft_cgi & rhs)
{
	_free_env();
	_env = rhs._env;
	_filename = rhs._filename;
	_response_body = rhs._response_body;
	_response = rhs._response;
	return *this;
}

// // // --- --- --- --- private methods --- --- --- --- // // //

void		ft_cgi::_construct_filename()
{
	
	_filename = " r.pwd + '/' + r.filename";
}

void					ft_cgi::_create_env()		// need
{ _env = NULL; }

void		ft_cgi::_free_env() {}					// need

void		ft_cgi::_parse_cgi() {}					// need

void		ft_cgi::_test_write_to_file()
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

bool		ft_cgi::_restore_fd_and_close(int pipe[2], int save[2])
{
	dup2(save[0], 0);
	dup2(save[1], 1);
	close(pipe[0]);
	close(pipe[1]);
	close(save[0]);
	close(save[1]);
	// _free_env();
	return false;
}

// // // --- --- --- - setters and getters - --- --- --- // // //

std::string const		ft_cgi::get_filename() const
	{ return _filename; }

void					ft_cgi::set_filename(str filename)
{
	_response_body = "";
	if(filename[0] == '/' or (filename[0] == '.' and filename[1] == '/'))
		_filename = filename;
	else
		_construct_filename();
}

// // // --- --- --- --- --- methods --- --- --- --- --- // // //

bool		ft_cgi::execute()
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
	// _parse_cgi();
	_restore_fd_and_close(fd_pipe, fd_save);
	_test_write_to_file();						// for test
	std::cout << _response_body << std::endl;	// for test
	return true;
}

// // // old temp
// bool		execute_cgi_script(std::string filename)
// {
// 	int				fd_pipe[2], status, ret;
// 	int				save_in = dup(0);	// для чтения
// 	int				save_out = dup(1);	// для записи
// 	char			buff[1024];
// 	std::string		response_body;

// 	pipe(fd_pipe);
// 	dup2(fd_pipe[0], 0);
// 	dup2(fd_pipe[1], 1);
// 	pid_t	pid = fork();
// 	if (pid == 0)
// 	{
// 		if(execve(filename.c_str(), NULL, NULL) == -1) // 3 argument = _env
// 		{
// 			close(fd_pipe[0]);
// 			close(fd_pipe[1]);
// 			close(save_in);
// 			close(save_out);
// 			// free _env();
// 			exit(127);
// 		}
// 	}
// 	wait(&status); //waitpid(pid, &status, WNOHANG);
// 	if(WEXITSTATUS(status) == 127)
// 	{
// 			dup2(save_in, 0);
// 			dup2(save_out, 1);
// 			close(fd_pipe[0]);
// 			close(fd_pipe[1]);
// 			close(save_in);
// 			close(save_out);
// 			// free _env();
// 			return false;
// 	}
//  	for(ret = 1024; ret == 1024 ; response_body += std::string(buff, ret))
// 		if((ret = read(fd_pipe[0], buff, 1024)) == -1)
// 		{
// 			dup2(save_in, 0);
// 			dup2(save_out, 1);
// 			close(fd_pipe[0]);
// 			close(fd_pipe[1]);
// 			close(save_in);
// 			close(save_out);
// 			// free _env();
// 			return false;
// 		}
// 	// // // --- test write to file	// ---
// 	int	file_fd = open("file.txt", O_RDWR | O_CREAT | O_APPEND, S_IWRITE | S_IREAD, 0755);
// 	if(file_fd == -1)
// 		std::cout << "error: file.txt not create" << std::endl;
// 	dup2(file_fd, 1);
// 	std::cout << "   ---" << std::endl;
// 	std::cout << response_body;
// 	close(file_fd);
// 	// // // --- test write to file	// ---
// 	dup2(save_in, 0);
// 	dup2(save_out, 1);
// 	close(fd_pipe[0]);
// 	close(fd_pipe[1]);
// 	close(save_in);
// 	close(save_out);
// 	std::cout << response_body << std::endl; // for test
// 	// parse_cgi();
// 	// free _env();
// 	return true;
// }