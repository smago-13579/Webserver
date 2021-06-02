/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smago <smago@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/24 12:58:15 by smago             #+#    #+#             */
/*   Updated: 2021/06/02 20:51:49 by smago            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int main()
{
	try {
		Server server;
		server.server_run();
	}
	catch (std::string str) {
		std::cout << str << std::endl;
		return 1;
	}
	return 0;
}
