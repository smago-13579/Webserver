/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smago <smago@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/23 13:42:25 by smago             #+#    #+#             */
/*   Updated: 2021/06/23 13:42:27 by smago            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "errors.hpp"

std::string error_page(int error_num)
{
    if (error_num == 400)
        return(error_400());
    if (error_num == 403)
        return(error_403());
    if (error_num == 404)
        return(error_404());
    if (error_num == 405)
        return(error_405());
    if (error_num == 413)
        return(error_413());
    if (error_num == 500)
        return(error_500());
    return("");
}

std::string error_400()
{
    std::string str;
    str = "<!DOCTYPE html><html><body><h1 style=\"font-size:300%;\">";
	str += "Error 400</h1><h2 style=\"font-size:160%;\">";
	str += "Bad Request</h2></body></html>";
    return (str);
}

std::string error_403()
{
    std::string str;
    str = "<!DOCTYPE html><html><body><h1 style=\"font-size:300%;\">";
	str += "Error 403</h1><h2 style=\"font-size:160%;\">";
	str += "Forbidden</h2></body></html>";
    return (str);
}

std::string error_404()
{
    std::string str;
    str = "<!DOCTYPE html><html><body><h1 style=\"font-size:300%;\">";
	str += "Error 404</h1><h2 style=\"font-size:160%;\">";
	str += "Not Found</h2></body></html>";
    return (str);
}

std::string error_405()
{
    std::string str;
    str = "<!DOCTYPE html><html><body><h1 style=\"font-size:300%;\">";
	str += "Error 405</h1><h2 style=\"font-size:160%;\">";
	str += "Method Not Allowed</h2></body></html>";
    return (str);
}

std::string error_413()
{
    std::string str;
    str = "<!DOCTYPE html><html><body><h1 style=\"font-size:300%;\">";
	str += "Error 413</h1><h2 style=\"font-size:160%;\">";
	str += "Payload Too Large</h2></body></html>";
    return (str);
}

std::string error_500()
{
    std::string str;
    str = "<!DOCTYPE html><html><body><h1 style=\"font-size:300%;\">";
	str += "Error 500</h1><h2 style=\"font-size:160%;\">";
	str += "Internal Server Error</h2></body></html>";
    return (str);
}
