
Response &	RequestHandler::handleRequest(Request & req)
{
	Response resp;

	if (_isRequestParsedCorrectly(resp, req) == false)
	{
		std::cout << "is bad req" << std::endl;
		if (req.getMethodEnum() == HEAD) {
			resp.body = "";
			resp.headers["content-length"] = "0";
		}
		return resp;
	}
	if (_isHostHeaderPresent(resp, req) == false)
	{
		std::cout << "is bad host\n";
		if (req.getMethodEnum() == HEAD) {
			resp.body = "";
			resp.headers["content-length"] = "0";
		}
		return resp;
	}

	_serverConfig = _choseServerConfig(req.getValueHeader("host"));
	if (_choseLocation(&_location, req.getRequestTarget()) == false)
	{
		resp = ErrorResponseFactory::create(ErrorResponseFactory::NOT_FOUND, _serverConfig.getErrorPageDir());
		if (req.getMethodEnum() == HEAD) {
			resp.body = "";
			resp.headers["content-length"] = "0";
		}
		return resp;
	}

	if (_isStatusLineValid(resp, req) == false)
	{
		std::cout << "is bad status line\n";
		if (req.getMethodEnum() == HEAD) {
			resp.body = "";
			resp.headers["content-length"] = "0";
		}
		return resp;
	}
	_pathComponents =  PathComponents
	(
		req.getRequestTarget(),
		_location.root,
		_location.index,
		std::string(_location.location).length()
	);

	if (_pathComponents.getPathType() == PathComponents::DIR &&
		_location.autoindex == "on" && req.getMethodEnum() == GET)
	{
		if (_isTerminatedWithSlash(req.getRequestTarget()) == false) {
			resp = _makeAddSlashToDirResponse(req.getRequestTarget());
			return resp;
		}
		resp = _makeDirectoryListingPageResponse(_pathComponents.getCurrentPath());
		return resp;
	}

	switch (req.getMethodEnum())
	{
		case HEAD:
			_handleHeadRequest(resp, req);
			break;
		case GET:
			_handleGetRequest(resp, req);
			break;
		case PUT:
			_handlePutRequest(resp, req);
			break;
		case POST:
			_handlePostRequest(resp, req);
			break;
		case UNSUPPORTED:
			break;
	}
	return resp;
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
//  	for(ret = 1024; ret == 1024 ; )
// 	{
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
// 		response_body += std::string(buff, ret)
// 	}
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