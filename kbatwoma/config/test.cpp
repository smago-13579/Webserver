# include "Config.hpp"

void    location(std::vector<Config::Location> &loc)
{
    for (std::vector<Config::Location>::iterator it = loc.begin(); it != loc.end(); it++)
    {
        std::cout << "  location: " << it->location << std::endl;
        std::cout << "  index: " << it->index << std::endl;
        std::cout << "  methods: " << std::endl;
        for (std::vector<size_t>::iterator it_m = (it->methods).begin(); it_m != (it->methods).end(); it_m++)
            std::cout << "    " << *it_m << std::endl;
        std::cout << "  root: " << it->root << std::endl;
        std::cout << "  autoindex: " << it->autoindex << std::endl;
        std::cout << "  max body: " << it->max_body << std::endl;
        std::cout << "  CGI_extension: " << it->CGI_extension << std::endl;
        std::cout << "  CGI_path: " << it->CGI_path << std::endl;
        std::cout << std::endl;
    }
}

int main()
{
    try{
        Config config("/home/venus/Desktop/Nastya/Webserver/kbatwoma/config/Den/conf/serv_easy.conf");
        std::vector<Config::Server> servers = config.getServers();
        for (std::vector<Config::Server>::iterator it = servers.begin(); it != servers.end(); it++)
        {
            std::cout << "IP: " << it->ip << std::endl;
            std::cout << "Port: " << it->port << std::endl;
            std::cout << "Server name: " << it->server_name << std::endl;
            std::cout << "Error page: " << it->error_page << std::endl;
            std::cout << "Locations: " << std::endl;
            location(it->locations);
            // std::cout << "Client body size: " << it->client_body_size << std::endl;
            std::cout << std::endl;
        }
    }
    catch(std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    return (0);
}