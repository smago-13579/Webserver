/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smago <smago@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/24 13:16:07 by smago             #+#    #+#             */
/*   Updated: 2021/06/17 20:22:09 by smago            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <sys/socket.h>
# include <sys/select.h>
# include <arpa/inet.h>
# include <iostream>
# include <unistd.h>
# include <cstdio>
# include <fcntl.h>
# include <list>
# include <vector>
# include <map>
# include <sstream>
# include <string>
# include <cstring>
# include <utility>
# include <ctime>
# include <fstream>
# include <dirent.h>

struct Location
{
	std::string         	location;
	std::string         	index;
	std::vector<size_t> 	methods;
	std::string         	root;
	bool                	autoindex;
	int			         	max_body;
	std::string				exec;
};

struct Settings
{
	std::string             ip;
	int                     port;
	std::string             server_name;
	std::string             error_page;
	std::vector<Location>   locations;
	std::string				redirect;

	Settings&			operator=(Settings& set)
	{
		if (this != &set)
		{
			this->ip = set.ip;
			this->port = set.port;
			this->server_name = set.server_name;
			this->error_page = set.error_page;
			this->locations = set.locations;
		}
		return (*this);
	}
};

/*		Only for positive numbers		*/
template <class T>
std::string itoa(T n)
{
	std::string str;
	if (n == 0)
		str.push_back('0');
	while (n != 0)
	{
		str.insert(str.begin(), (n % 10 + 48));
		n /= 10;
	}
	return str;
}


#endif 
