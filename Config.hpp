/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbatwoma <kbatwoma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/08 10:30:07 by kbatwoma          #+#    #+#             */
/*   Updated: 2021/06/17 18:28:19 by kbatwoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <iostream>
# include <string>
# include <vector>
# include <fstream>
# include <limits>
# include <exception>

# define GET 0
# define POST 1
# define DELETE 2
# define PUT 3

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
            int                 max_body;
            std::string         exec;
        };
        struct Server
        {
            Server();

            std::string             ip;
            int                     port;
            std::string             server_name;
            std::string             error_page;
            std::vector<Location>   locations;
            std::string             redirect;
        };

        class Missing_field : public std::exception
        {
            const char* what() const throw();
        };
        class Syntax_error : public std::exception
        {
            const char* what() const throw();
        };
        class File_error : public std::exception
        {
            const char* what() const throw();
        };
        class Data_error : public std::exception
        {
            const char* what() const throw();
        };

        std::vector<Server> const   &getServers();
        void                        parser();

    private:
        Config();

        std::vector<Server> _servers;
        std::string         _config_line;
        std::string         _server_line;
        std::string         _location_line;

        Server      *parser_server();
        Location    *parser_location();
        void        check_braces();
};

#endif
