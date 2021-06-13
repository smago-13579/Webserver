#ifndef CGI_HANDLER_HPP
# define CGI_HANDLER_HPP

# include <iostream>	// string wait cout endl
# include <unistd.h>	// pipe execve dup read close
# include <fcntl.h>		// open
# include <map>			// map

# define buffer_size 1024

class Request {};		// test

class cgi_handler
{
public:
	typedef std::string			str;

private:
	char **	_env;
	str		_filename;
	str		_response_body;
	int		_status_code;
	str		_str_status_code;
	str		_str_content_type;


	// private methods
	char *	_ft_strjoin(str str1, str str2, str str3, str str4);
	void	_create_env(Request * request);
	void	_construct_filename();// need ?
	void	_free_env();
	void	_parse_cgi();
	void	_test_write_to_file();
	bool	_restore_fd_and_close(int pipe[2], int save[2]);

public:
	cgi_handler();
	cgi_handler(str filename);
	cgi_handler(Request request, str filename);
	~cgi_handler();
	cgi_handler(const cgi_handler & copy);
	cgi_handler &	operator= (const cgi_handler & rhs);

	// setters and getters
	void		set_request(Request request);
	void		set_filename(str filename);
	str const	get_filename() const;
	str const	get_response_body() const;
	int const	get_status_code() const;
	str const	get_str_content_type() const;
	str const	get_str_status_code() const;

	// methods
	bool	execute();

};

#endif