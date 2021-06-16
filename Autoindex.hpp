#ifndef AUTIONDEX_HPP
# define AUTIONDEX_HPP

# include <iostream>

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

        std::string _body;
};

#endif