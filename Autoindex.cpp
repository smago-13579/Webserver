/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Autoindex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smago <smago@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/17 11:04:47 by kbatwoma          #+#    #+#             */
/*   Updated: 2021/06/17 20:05:06 by smago            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Autoindex.hpp"

/**********************/
/*                    */
/*   Coplien's form   */
/*                    */
/**********************/
Autoindex::Autoindex(std::string &path) : _body(std::string()), _folder_contents(std::vector<std::string>())
{
    DIR *dir;
    dirent *ent;
    if ((dir = opendir(path.c_str())) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            if (std::string(ent->d_name) != ".DS_Store")
                _folder_contents.push_back(ent->d_name);
        }
        closedir(dir);
    }
    else
    {
        std::cout << "error" << std::endl;
    }
    _create_body();
}

Autoindex::Autoindex(Autoindex const &a) : _body(a._body), _folder_contents(a._folder_contents)
{    
}

Autoindex   &Autoindex::operator=(Autoindex const &a)
{
    if (this != &a)
    {
        _body = a._body;
        _folder_contents = a._folder_contents;
    }
    return (*this);
}

Autoindex::~Autoindex()
{
}

/***************/
/*             */
/*   Getters   */
/*             */
/***************/

std::string const   &Autoindex::getBody()
{
    return (_body);
}

/*******************/
/*                 */
/*   Create body   */
/*                 */
/*******************/
void    Autoindex::_create_body()
{
    _body += "<!DOCTYPE html>\n";
    _body += "<html lang=\"en\">\n";
    _body += "<head>\n";
    _body += "\t<meta charset=\"UTF-8\">\n";
    _body += "\t<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n";
    _body += "\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
    _body += "\t<link rel=\"stylesheet\" href=\"style.css\">\n";
    _body += "\t<title>Autoindex</title>\n";
    _body += "</head>\n";
    _body += "<body class=\"error error_page autoindex\">\n";
    for (std::vector<std::string>::iterator it = _folder_contents.begin();
                                        it != _folder_contents.end(); it++)
    {
        _body += "\t<a class=\"color_autoindex\" href=\"";
        _body += *it;
        _body += "\">";
        _body += *it;
        _body += "</a>\n";
    }
    _body += "</body>\n";
    _body += "</html>";
}