/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_conf.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: monie <monie@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/12 16:17:52 by monie             #+#    #+#             */
/*   Updated: 2021/05/18 19:15:20 by monie            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <fstream>
#include "config.hpp"



int main(int argc, char** argv) {
	Config config;
	
	std::string file;
	if(argc == 2)
		file = argv[1];
	else
		file = "serv.conf";
	config.loop(file);
	//config.see_config();
	return 0;
}