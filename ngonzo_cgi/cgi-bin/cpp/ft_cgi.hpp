#ifndef FT_CGI_HPP
# define FT_CGI_HPP

# include <iostream>	// string wait cout endl
# include <unistd.h>	// pipe execve dup read close
# include <fcntl.h>		// open

# define buffer_size 1024

class ft_cgi
{
public:
	typedef std::string			str;

private:
	str		*_env;				// need
	str		_filename;
	str		_response_body;
	str		_response;			// need

	// private methods
	void	_create_env();		// need
	void	_construct_filename();//need
	void	_free_env();		// need
	void	_parse_cgi();		// need
	void	_test_write_to_file();
	

public:
	ft_cgi();
	ft_cgi(str filename);
	~ft_cgi();
	ft_cgi(const ft_cgi & copy);
	ft_cgi &	operator= (const ft_cgi & rhs);

	// setters and getters
	str const	get_filename() const;
	void		set_filename(str filename);

	// methods
	bool	execute();

};

#endif