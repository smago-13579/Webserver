/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbatwoma <kbatwoma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/08 10:30:18 by kbatwoma          #+#    #+#             */
/*   Updated: 2021/06/08 15:56:39 by kbatwoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Config.hpp"

Config::Location::Location() : location(std::string()), index(std::string()), methods(std::vector<size_t>()), root(std::string()), autoindex(0), max_body(std::string())
{}

Config::Server::Server() : ip(std::string()), port(0), server_name(std::string()), error_page(std::string()), locations(std::vector<Location>())
{} 

Config::Config()
{}

Config::Config(std::string config_name)
{
    std::string     line;
    std::ifstream   config(config_name);
    int             len;

    if (config.is_open())
        while (getline(config, line))
        {
            len = line.length();
            for (int i = 0; i < len; i++)
                if (line[i] != ' ' && line[i] != '\t')
                    _config_line += line[i];
        }
    config.close();
    parser();

    //checker();
}

Config::~Config()
{
    // возможно, нужно будет чистить new
}

std::vector<Config::Server> const   &Config::getServers()
{
    return(_servers);
}


void    Config::parser()
{
    size_t  pos_begin = 0;
    size_t  pos_end = 0;
    size_t  flag = 0;

    if ((pos_begin = _config_line.find("server{")) == _config_line.npos)
        return;
    pos_begin += 7;
    while (flag == 0)
    {
        if ((pos_end = _config_line.find("server{", pos_begin)) == _config_line.npos)
            flag = 1;
        _server_line = std::string(_config_line, pos_begin, pos_end - pos_begin);
        _servers.push_back(*parser_server());
        pos_begin = pos_end + 7;
    }
}

Config::Server  *Config::parser_server()
{
    size_t          pos_begin = 0;
    size_t          pos_end = 0;
    std::string     help_line;
    Config::Server  *point_to_serv = new Config::Server;

    //listen
    if ((pos_begin = _server_line.find("listen")) != _server_line.npos)
    {
        pos_begin += 6;
        if ((pos_end = _server_line.find(":", pos_begin)) == _server_line.npos)
            return (point_to_serv);//error
        (*point_to_serv).ip = std::string(_server_line, pos_begin, pos_end - pos_begin);
        pos_begin = pos_end + 1;
        if ((pos_end = _server_line.find(";", pos_begin)) == _server_line.npos)
            return (point_to_serv);//error
        help_line = std::string(_server_line, pos_begin, pos_end - pos_begin);
        (*point_to_serv).port = atoi(help_line.c_str());
    }
    //если нет, то сказать об ошибке

    //server_name
    pos_begin = 0;
    if ((pos_begin = _server_line.find("server_name")) != _server_line.npos)
    {
        pos_begin += 11;
        if ((pos_end = _server_line.find(";", pos_begin)) == _server_line.npos)
            return (point_to_serv);//error
        (*point_to_serv).server_name = std::string(_server_line, pos_begin, pos_end - pos_begin);
    }
    //если нет, то сказать об ошибке

    //error_page
    pos_begin = 0;
    if ((pos_begin = _server_line.find("error_page")) != _server_line.npos)
    {
        pos_begin += 10;
        if ((pos_end = _server_line.find(";", pos_begin)) == _server_line.npos)
            return (point_to_serv);//error
        (*point_to_serv).error_page = std::string(_server_line, pos_begin, pos_end - pos_begin);
    }
    //если нет, то сказать об ошибке
    
    //locations
    pos_begin = 0;
    while (true)
    {
        if ((pos_begin = _server_line.find("location", pos_begin)) == _server_line.npos)
            break;
        pos_begin += 8;
        if ((pos_end = _server_line.find("}", pos_begin)) == _server_line.npos)
            return (point_to_serv);//error
        _location_line = std::string(_server_line, pos_begin, pos_end - pos_begin);
        (*point_to_serv).locations.push_back(*parser_location());
        pos_begin = pos_end + 1;
    }

    //client_body_size;

    return (point_to_serv);
}

Config::Location  *Config::parser_location()
{
    size_t              pos_begin = 0;
    size_t              pos_start;
    size_t              pos_end = 0;
    std::string         help_line;
    Config::Location    *point_to_location = new Config::Location;
    
    //location
    if ((pos_end = _location_line.find("{")) == _location_line.npos)
            return (point_to_location);//error
    (*point_to_location).location = std::string(_location_line, pos_begin, pos_end - pos_begin);
    pos_begin = pos_end + 1;
    pos_start = pos_begin;

    //index
    if ((pos_begin = _location_line.find("index")) != _location_line.npos)
    {
        pos_begin += 5;
        if ((pos_end = _location_line.find(";", pos_begin)) == _location_line.npos)
            return (point_to_location);//error
        (*point_to_location).index = std::string(_location_line, pos_begin, pos_end - pos_begin);
    }
    //если нет, то сказать об ошибке

    //methods
    pos_begin = pos_start;
    if ((pos_begin = _location_line.find("methods")) != _location_line.npos)
    {
        pos_begin += 7;
        if ((pos_end = _location_line.find(";", pos_begin)) == _location_line.npos)
            return (point_to_location);//error
        help_line = std::string(_location_line, pos_begin, pos_end - pos_begin);
        size_t flag = 0;
        std::string method;
        pos_begin = 0;
        while (flag == 0)
        {
            if ((pos_end = help_line.find(",", pos_begin)) == help_line.npos)
                flag = 1;
            method = std::string(help_line, pos_begin, pos_end - pos_begin);
            if (method == "GET")
                (*point_to_location).methods.push_back(GET);
            else if (method == "POST")
                (*point_to_location).methods.push_back(POST);
            else if (method == "DELETE")
                (*point_to_location).methods.push_back(DELETE);
            else
                return(point_to_location);//error
            pos_begin = pos_end + 1;
        }
    }

    //root
    pos_begin = pos_start;
    if ((pos_begin = _location_line.find("root")) != _location_line.npos)
    {
        pos_begin += 4;
        if ((pos_end = _location_line.find(";", pos_begin)) == _location_line.npos)
            return (point_to_location);//error
        (*point_to_location).root = std::string(_location_line, pos_begin, pos_end - pos_begin);
    }

    //autoindex
    pos_begin = pos_start;
    if ((pos_begin = _location_line.find("autoindex")) != _location_line.npos)
    {
        pos_begin += 4;
        if ((pos_end = _location_line.find(";", pos_begin)) == _location_line.npos)
            return (point_to_location);//error
        help_line = std::string(_location_line, pos_begin, pos_end - pos_begin);
        if (help_line == "on")
            (*point_to_location).autoindex = ON;
        else if (help_line == "off")
            (*point_to_location).autoindex = OFF;
        else
            return(point_to_location);//error
    }

    //max_body
    return (point_to_location);
}
