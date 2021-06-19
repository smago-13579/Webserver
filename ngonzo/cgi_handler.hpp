#ifndef CGI_HANDLER_HPP
# define CGI_HANDLER_HPP

# include <iostream>	// string wait cout endl
# include <unistd.h>	// pipe execve dup read close
# include <fcntl.h>		// open
# include <map>			// map
# include <fstream>
# include <string>
# include <vector>
# include <sys/stat.h>
# include <sys/wait.h>
# include <cstring>

# define buffer_size 1024

class cgi_handler
{
public:
	typedef std::string     str;

private:
	char **	_env;
	str		_filename;
	str		_response_body;
	int		_status_code;
	str		_str_status_code;
	str		_str_content_type;
    str		_root;
    str     _exec;
    std::ofstream   _req_body;
    str		_root_req_body;


	// private methods
	char *	_string_to_char(std::string str);
	void	_parse_env(std::vector<str> env);
	void	_free_env();
	void	_parse_cgi();
	void	_test_write_to_file();
	bool	_restore_fd_and_close(int pipe[2], int save[2]);

public:
	cgi_handler();
	cgi_handler(str filename);
	cgi_handler(std::vector<std::string> env, str & root);
	~cgi_handler();
	cgi_handler(const cgi_handler & copy);
	cgi_handler &	operator= (const cgi_handler & rhs);

	// setters and getters
	str const	get_filename() const;
	
	str const	get_response_body() const;
	int const	get_status_code() const;
	str const	get_str_content_type() const;
	str const	get_str_status_code() const;

	// methods
	bool	execute();
    bool	execute_pipe();
    bool	execute_tester();
    void    req_body_to_fd(str & request_body);

};

#endif