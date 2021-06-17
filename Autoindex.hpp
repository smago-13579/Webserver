/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Autoindex.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbatwoma <kbatwoma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/17 11:05:03 by kbatwoma          #+#    #+#             */
/*   Updated: 2021/06/17 13:04:27 by kbatwoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUTIONDEX_HPP
# define AUTIONDEX_HPP

# include <iostream>
# include <vector>
# include <dirent.h>

class Autoindex
{
    public:
        Autoindex(std::string &);
        Autoindex(Autoindex const &);
        Autoindex &operator=(Autoindex const &);
        ~Autoindex();

        std::string const &getBody();

    private:
        Autoindex();
        void    _create_body();

        std::string                 _body;
        std::vector<std::string>    _folder_contents;
};

#endif