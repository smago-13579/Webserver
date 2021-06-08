/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbatwoma <kbatwoma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/08 10:30:07 by kbatwoma          #+#    #+#             */
/*   Updated: 2021/06/08 15:26:39 by kbatwoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <iostream>
# include <string>
# include <vector>
# include <fstream>

# define GET 0
# define POST 1
# define DELETE 2

# define ON 1
# define OFF 0

class Config
{
    public:
        Config(std::string file_name);
        Config(Config const &);
        Config &operator=(Config const &);
        ~Config();

        struct Location
        {
            Location();

            std::string         location;
            std::string         index;
            std::vector<size_t> methods;
            std::string         root;
            bool                autoindex;
            std::string         max_body;
            //...
        };
        struct Server
        {
            Server();

            std::string             ip;
            int                     port;
            std::string             server_name;
            std::string             error_page;
            std::vector<Location>   locations;
            int                     client_body_size;
            
        };

        std::vector<Server> const   &getServers();
        void                        parser();

    private:
        Config();

        std::vector<Server> _servers;
        std::string         _config_line;
        std::string         _server_line;
        std::string         _location_line;
        Server              _current_server;

        Server      *parser_server();
        Location    *parser_location();
};

#endif