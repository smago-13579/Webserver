/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smago <smago@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/24 12:58:15 by smago             #+#    #+#             */
/*   Updated: 2021/06/10 19:16:01 by smago            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int main(int ac, char** av)
{
	if (ac == 2) {
		try {
			Server server(av[1]);
			server.server_run();
		}
		catch (std::string str) {
			std::cout << str << std::endl;
			return 1;
		}
	}
	else
		std::cout << "Wrong number of arguments\n";
	return 0;
}
