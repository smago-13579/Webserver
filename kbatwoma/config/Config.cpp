/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbatwoma <kbatwoma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/08 10:30:18 by kbatwoma          #+#    #+#             */
/*   Updated: 2021/06/13 15:25:44 by kbatwoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Config.hpp"

/*********************************************/
/*                                           */
/*   Constructors | Operator= | Destructor   */
/*                                           */
/*********************************************/
Config::Location::Location() : location(std::string()), index(std::string()),
                            methods(std::vector<size_t>()), root(std::string()),
                            autoindex(OFF), max_body(std::numeric_limits<int>::max() - 1),
                            CGI_extension(std::string()), CGI_path(std::string())
{}

Config::Server::Server() : ip(std::string()), port(int()), server_name(std::string()),
                        error_page(std::string()), locations(std::vector<Location>())
{}

Config::Config(std::string config_name) : _servers(std::vector<Server>()),
                                        _config_line(std::string()),
                                        _server_line(std::string()),
                                        _location_line(std::string())
{
    std::string     line;
    std::ifstream   config(config_name);
    int             len;

    if (config.is_open())
    {
        while (getline(config, line))
        {
            len = line.length();
            for (int i = 0; i < len; i++)
                if (line[i] != ' ' && line[i] != '\t')
                    _config_line += line[i];
        }
        config.close();
    }
    else
        throw(Config::File_error());
    parser();
}

Config::Config(Config const &a) : _servers(a._servers), _config_line(a._config_line),
                                _server_line(a._server_line), _location_line(a._location_line)
{
}

Config  &Config::operator=(Config const &a)
{
    if (this != &a)
    {
        _servers = a._servers;
        _config_line = a._config_line;
        _server_line = a._server_line;
        _location_line = a._location_line;
    }
    return (*this);
}

Config::~Config()
{
}

/******************/
/*                */
/*   Exceptions   */
/*                */
/******************/
const char* Config::Missing_field::what() const throw()
{ return ("ERROR -> configuration file: Missing a required field"); }

const char* Config::Syntax_error::what() const throw()
{ return ("ERROR -> configuration file: Syntax error"); }

const char* Config::File_error::what() const throw()
{ return ("ERROR -> configuration file: Unable to read the file"); }

const char* Config::Data_error::what() const throw()
{ return ("ERROR -> configuration file: Data error"); }

/***************/
/*             */
/*   Getters   */
/*             */
/***************/
std::vector<Config::Server> const   &Config::getServers()
{
    return(_servers);
}

/**************/
/*            */
/*   Parser   */
/*            */
/**************/
void    Config::check_braces()
{
    std::string help;
    size_t flag = 0;
    size_t i = 0;
    size_t open;
    size_t close;
    while (i != _config_line.npos)
    {
        i = (flag == 0) ? 0 : i + 1;
        if (((open = _config_line.find("{", i)) != _config_line.npos
                && (close = _config_line.find("}", i)) != _config_line.npos) ||
            ((open = _config_line.find("{", i)) != _config_line.npos
                && (close = _config_line.find("}", i)) == _config_line.npos) ||
            ((open = _config_line.find("{", i)) == _config_line.npos
                && (close = _config_line.find("}", i)) != _config_line.npos))
        {
            i = open > close ? close : open;
            help += _config_line[i];
        }
        else
            i = _config_line.npos;
        flag++;
    }
    while (help.find("}") != help.npos)
        if ((i = help.find("}")) != help.npos)
        {
            if (help[i - 1])
                help.erase(i - 1, 2);
            else
                throw(Config::Syntax_error());
        }
    if (!help.empty())
        throw(Config::Syntax_error());
}

void    Config::parser()
{
    size_t  pos_begin = 0;
    size_t  pos_end = 0;
    size_t  flag = 0;
    Config::Server *serv;

    check_braces();
    if ((pos_begin = _config_line.find("server{")) == _config_line.npos)
        throw(Config::Missing_field());
    pos_begin += 7;
    while (flag == 0)
    {
        if ((pos_end = _config_line.find("server{", pos_begin)) == _config_line.npos)
            flag = 1;
        _server_line = std::string(_config_line, pos_begin, pos_end - pos_begin);
        serv = parser_server();
        size_t i = 0;
        for (; i < _servers.size(); i++)
            if (_servers[i].ip == serv->ip && _servers[i].port == serv->port)
                break;
        if (i == _servers.size())
            _servers.push_back(*serv);
        delete serv;
        pos_begin = pos_end + 7;
    }
}

Config::Server  *Config::parser_server()
{
    size_t          pos_begin = 0;
    size_t          pos_end = 0;
    std::string     help_line;
    Config::Server  *point_to_serv = new Config::Server;

    /**************/
    /*   listen   */ //обязательное поле
    /**************/
    if ((pos_begin = _server_line.find("listen")) != _server_line.npos)
    {
        if (_server_line[pos_begin - 1] && (_server_line[pos_begin - 1] != ';'
                                        && _server_line[pos_begin - 1] != '}'))
        {
            delete point_to_serv;
            throw(Config::Syntax_error());
        }
        pos_begin += 6;
        if ((pos_end = _server_line.find(":", pos_begin)) == _server_line.npos)
        {
            delete point_to_serv;
            throw(Config::Syntax_error());
        }
        (*point_to_serv).ip = std::string(_server_line, pos_begin, pos_end - pos_begin);
        pos_begin = pos_end + 1;
        if ((pos_end = _server_line.find(";", pos_begin)) == _server_line.npos)
        {
            delete point_to_serv;
            throw(Config::Syntax_error());
        }
        help_line = std::string(_server_line, pos_begin, pos_end - pos_begin);
        (*point_to_serv).port = atoi(help_line.c_str());
        if ((*point_to_serv).port < 0)
        {
            delete point_to_serv;
            throw(Config::Data_error());
        }
    }
    else
    {
        delete point_to_serv;
        throw(Config::Missing_field());
    }

    /*******************/
    /*   server_name   */ //не обязательный параметр 
    /*******************/
    pos_begin = 0;
    if ((pos_begin = _server_line.find("server_name")) != _server_line.npos)
    {
        if (_server_line[pos_begin - 1] && (_server_line[pos_begin - 1] != ';'
                                        && _server_line[pos_begin - 1] != '}'))
        {
            delete point_to_serv;
            throw(Config::Syntax_error());
        }
        pos_begin += 11;
        if ((pos_end = _server_line.find(";", pos_begin)) == _server_line.npos)
        {
            delete point_to_serv;
            throw(Config::Syntax_error());
        }
        (*point_to_serv).server_name = std::string(_server_line, pos_begin, pos_end - pos_begin);
    }

    /******************/
    /*   error_page   */ //обязательное поле
    /******************/
    pos_begin = 0;
    if ((pos_begin = _server_line.find("error_page")) != _server_line.npos)
    {
        if (_server_line[pos_begin - 1] && (_server_line[pos_begin - 1] != ';'
                                        && _server_line[pos_begin - 1] != '}'))
        {
            delete point_to_serv;
            throw(Config::Syntax_error());
        }
        pos_begin += 10;
        if ((pos_end = _server_line.find(";", pos_begin)) == _server_line.npos)
        {
            delete point_to_serv;
            throw(Config::Syntax_error());
        }
        (*point_to_serv).error_page = std::string(_server_line, pos_begin, pos_end - pos_begin);
    }
    else
    {
        delete point_to_serv;
        throw(Config::Missing_field());
    }
    
    /*****************/
    /*   locations   */ //обязательное поле
    /*****************/
    pos_begin = 0;
    int count_locations = 0;
    Config::Location *loc;
    while (true)
    {
        if ((pos_begin = _server_line.find("location", pos_begin)) == _server_line.npos)
            break;
        count_locations++;
        if (_server_line[pos_begin - 1] && (_server_line[pos_begin - 1] != ';'
                                        && _server_line[pos_begin - 1] != '}'))
        {
            delete point_to_serv;
            throw(Config::Syntax_error());
        }
        pos_begin += 8;
        if ((pos_end = _server_line.find("}", pos_begin)) == _server_line.npos)
        {
            delete point_to_serv;
            throw(Config::Syntax_error());
        }
        _location_line = std::string(_server_line, pos_begin, pos_end - pos_begin);
        loc = parser_location();
        (*point_to_serv).locations.push_back(*loc);
        delete loc;
        pos_begin = pos_end + 1;
    }
    if (count_locations == 0)
    {
        delete point_to_serv;
        throw(Config::Missing_field());
    }

    return (point_to_serv);
}

Config::Location  *Config::parser_location()
{
    size_t              pos_begin = 0;
    size_t              pos_start;
    size_t              pos_end = 0;
    std::string         help_line;
    Config::Location    *point_to_location = new Config::Location;
    
    
    /****************/
    /*   location   */ //обязательное поле
    /****************/
    if ((pos_end = _location_line.find("{")) == _location_line.npos)
    {
        delete point_to_location;
        throw(Config::Syntax_error());
    }
    (*point_to_location).location = std::string(_location_line, pos_begin, pos_end - pos_begin);
    if ((*point_to_location).location.empty())
    {
        delete point_to_location;
        throw(Config::Missing_field());
    }
    pos_begin = pos_end + 1;
    pos_start = pos_begin;

    /*************/
    /*   index   */ // не обязательное поле
    /*************/
    if ((pos_begin = _location_line.find("index")) != _location_line.npos)
    {
        if (_location_line[pos_begin - 1] && (_location_line[pos_begin - 1] != ';'
                                        && _location_line[pos_begin - 1] != '}'))
        {
            delete point_to_location;
            throw(Config::Syntax_error());
        }
        pos_begin += 5;
        if ((pos_end = _location_line.find(";", pos_begin)) == _location_line.npos)
        {
            delete point_to_location;
            throw(Config::Syntax_error());
        }
        (*point_to_location).index = std::string(_location_line, pos_begin, pos_end - pos_begin);
    }

    /***************/
    /*   methods   */ // не обязательное поле
    /***************/
    pos_begin = pos_start;
    if ((pos_begin = _location_line.find("methods")) != _location_line.npos)
    {
        pos_begin += 7;
        if ((pos_end = _location_line.find(";", pos_begin)) == _location_line.npos)
        {
            delete point_to_location;
            throw(Config::Syntax_error());
        }
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
            {
                delete point_to_location;
                throw(Config::Data_error());
            }
            pos_begin = pos_end + 1;
        }
    }

    /************/
    /*   root   */ //обязательное поле
    /************/
    pos_begin = pos_start;
    if ((pos_begin = _location_line.find("root")) != _location_line.npos)
    {
        pos_begin += 4;
        if ((pos_end = _location_line.find(";", pos_begin)) == _location_line.npos)
        {
            delete point_to_location;
            throw(Config::Syntax_error());
        }
        (*point_to_location).root = std::string(_location_line, pos_begin, pos_end - pos_begin);
    }
    else
    {
        delete point_to_location;
        throw(Config::Missing_field());
    }

    /*****************/
    /*   autoindex   */ //не обязательное поле - по умолчанию на off
    /*****************/
    pos_begin = pos_start;
    if ((pos_begin = _location_line.find("autoindex")) != _location_line.npos)
    {
        pos_begin += 4;
        if ((pos_end = _location_line.find(";", pos_begin)) == _location_line.npos)
        {
            delete point_to_location;
            throw(Config::Syntax_error());
        }
        help_line = std::string(_location_line, pos_begin, pos_end - pos_begin);
        if (help_line == "on")
            (*point_to_location).autoindex = ON;
        else if (help_line == "off")
            (*point_to_location).autoindex = OFF;
        else
        {
            delete point_to_location;
            throw(Config::Data_error());
        }
    }

    /****************/
    /*   max_body   */ //не обязательное поле - по умолчанию на max int
    /****************/
    pos_begin = pos_start;
    if ((pos_begin = _location_line.find("max_body")) != _location_line.npos)
    {
        pos_begin += 8;
        if ((pos_end = _location_line.find(";", pos_begin)) == _location_line.npos)
        {
            delete point_to_location;
            throw(Config::Syntax_error());
        }
        help_line = std::string(_location_line, pos_begin, pos_end - pos_begin);
        (*point_to_location).max_body = atoi(help_line.c_str());
        if ((*point_to_location).max_body < 0)
        {
            delete point_to_location;
            throw(Config::Data_error());
        }
    }

    /*********************/
    /*   CGI_extension   */ //не обязательное поле
    /*********************/
    pos_begin = pos_start;
    if ((pos_begin = _location_line.find("CGI_extension")) != _location_line.npos)
    {
        pos_begin += 13;
        if ((pos_end = _location_line.find(";", pos_begin)) == _location_line.npos)
        {
            delete point_to_location;
            throw(Config::Syntax_error());
        }
        (*point_to_location).CGI_extension = std::string(_location_line, pos_begin, pos_end - pos_begin);
    }

    /****************/
    /*   CGI_path   */ //не обязательное поле
    /****************/
    pos_begin = pos_start;
    if ((pos_begin = _location_line.find("CGI_path")) != _location_line.npos)
    {
        pos_begin += 8;
        if ((pos_end = _location_line.find(";", pos_begin)) == _location_line.npos)
        {
            delete point_to_location;
            throw(Config::Syntax_error());
        }
        (*point_to_location).CGI_path = std::string(_location_line, pos_begin, pos_end - pos_begin);
    }
    if (((*point_to_location).CGI_extension.empty() && !((*point_to_location).CGI_path.empty()))
        || (!((*point_to_location).CGI_extension.empty()) && (*point_to_location).CGI_path.empty()))
    {
        delete point_to_location;
        throw(Config::Missing_field());
    }
    
    return (point_to_location);
}
