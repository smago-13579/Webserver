/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smago <smago@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/24 13:16:07 by smago             #+#    #+#             */
/*   Updated: 2021/06/11 22:06:16 by smago            ###   ########.fr       */
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

# include "Server.hpp"
# include "Request.hpp"
// # include "Response.hpp"
# include "kbatwoma/config/Config.hpp"
# include "Socket.hpp"


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
