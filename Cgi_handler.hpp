/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi_handler.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smago <smago@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/23 13:41:54 by smago             #+#    #+#             */
/*   Updated: 2021/06/23 13:41:56 by smago            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HANDLER_HPP
# define CGI_HANDLER_HPP

# include <iostream>	
# include <unistd.h>	
# include <fcntl.h>	
# include <map>		
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
	char**			_env;
	str				_filename;
	str				_response_body;
	str				_root;
	str				_exec;
	std::ofstream	_req_body;
	str				_root_req_body;


	// private methods
	char *	_string_to_char(std::string str);
	void	_parse_env(std::vector<str> env);
	void	_free_env();
	void	_parse_cgi();
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

	// methods
	bool	execute();
};

#endif