// // // old temp
bool		execute_cgi_script(std::string filename)
{
	int				fd_pipe[2], status, ret;
	int				save_in = dup(0);	// для чтения
	int				save_out = dup(1);	// для записи
	char			buff[1024];
	std::string		response_body;

	pipe(fd_pipe);
	dup2(fd_pipe[0], 0);
	dup2(fd_pipe[1], 1);
	pid_t	pid = fork();
	if (pid == 0)
	{
		if(execve(filename.c_str(), NULL, NULL) == -1) // 3 argument = _env
		{
			close(fd_pipe[0]);
			close(fd_pipe[1]);
			close(save_in);
			close(save_out);
			// free _env();
			exit(127);
		}
	}
	wait(&status); //waitpid(pid, &status, WNOHANG);
	if(WEXITSTATUS(status) == 127)
	{
			dup2(save_in, 0);
			dup2(save_out, 1);
			close(fd_pipe[0]);
			close(fd_pipe[1]);
			close(save_in);
			close(save_out);
			// free _env();
			return false;
	}
 	for(ret = 1024; ret == 1024 ; )
	{
		if((ret = read(fd_pipe[0], buff, 1024)) == -1)
		{
			dup2(save_in, 0);
			dup2(save_out, 1);
			close(fd_pipe[0]);
			close(fd_pipe[1]);
			close(save_in);
			close(save_out);
			// free _env();
			return false;
		}
		response_body += std::string(buff, ret)
	}
	// // // --- test write to file	// ---
	int	file_fd = open("file.txt", O_RDWR | O_CREAT | O_APPEND, S_IWRITE | S_IREAD, 0755);
	if(file_fd == -1)
		std::cout << "error: file.txt not create" << std::endl;
	dup2(file_fd, 1);
	std::cout << "   ---" << std::endl;
	std::cout << response_body;
	close(file_fd);
	// // // --- test write to file	// ---
	dup2(save_in, 0);
	dup2(save_out, 1);
	close(fd_pipe[0]);
	close(fd_pipe[1]);
	close(save_in);
	close(save_out);
	std::cout << response_body << std::endl; // for test
	// parse_cgi();
	// free _env();
	return true;
}