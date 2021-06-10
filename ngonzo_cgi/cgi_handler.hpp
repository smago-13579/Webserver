#ifndef CGI_HANDLER_HPP
# define CGI_HANDLER_HPP

# include <iostream>	// string wait cout endl
# include <unistd.h>	// pipe execve dup read close
# include <fcntl.h>		// open
# include <map>			// map

# define buffer_size 1024

class Request {};		// test
class t_client_addr {};	// test

class cgi_handler
{
public:
	typedef std::string			str;
	typedef char **				cpp;
	typedef char *				cp;

private:
	cpp		_env;
	str		_filename;
	str		_response_body;

	// private methods
	cp		_ft_strjoin(str str1, str str2, str str3, str str4);
	void	_create_env(Request * request, t_client_addr * client_addr);
	void	_construct_filename();// need ?
	void	_free_env();
	void	_parse_cgi();		// need
	void	_test_write_to_file();
	bool	_restore_fd_and_close(int pipe[2], int save[2]);

public:
	cgi_handler() {}
	cgi_handler(str filename);
	cgi_handler(Request request, t_client_addr client_addr);
	cgi_handler(Request request, t_client_addr client_addr, str filename);
	~cgi_handler();
	cgi_handler(const cgi_handler & copy);
	cgi_handler &	operator= (const cgi_handler & rhs);

	// setters and getters
	str const	get_filename() const;
	void		set_filename(str filename);
	void		get_response_body();	//need
	void		get_status_code();		//need
	void		get_str_content_type();	//need
	void		get_str_status_code();	//need

	// methods
	bool	execute();

};

#endif