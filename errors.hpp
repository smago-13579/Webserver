#ifndef ERRORS_HPP
# define ERRORS_HPP

# include <iostream>

std::string &error_page(int error_num);
std::string &error_400();
std::string &error_403();
std::string &error_404();
std::string &error_405();
std::string &error_413();
std::string &error_500();

#endif