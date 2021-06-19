# include "../Config.hpp"
# include "../Autoindex.hpp"
# include <unistd.h>

// void    location(std::vector<Config::Location> &loc)
// {
//     for (std::vector<Config::Location>::iterator it = loc.begin(); it != loc.end(); it++)
//     {
//         std::cout << "  location: " << it->location << std::endl;
//         std::cout << "  index: " << it->index << std::endl;
//         std::cout << "  methods: " << std::endl;
//         for (std::vector<size_t>::iterator it_m = (it->methods).begin(); it_m != (it->methods).end(); it_m++)
//             std::cout << "    " << *it_m << std::endl;
//         std::cout << "  root: " << it->root << std::endl;
//         std::cout << "  autoindex: " << it->autoindex << std::endl;
//         std::cout << "  max body: " << it->max_body << std::endl;
//         std::cout << "  CGI_extension: " << it->CGI_extension << std::endl;
//         std::cout << "  CGI_path: " << it->CGI_path << std::endl;
//         std::cout << std::endl;
//     }
// }

void    put(std::string &str)
{
    std::string file_path = str;
	std::string file_name = std::string(file_path, file_path.rfind("/") + 1, file_path.npos - file_path.rfind("/") - 1);
	
			std::ofstream file;
            file.open(file_name);
			if (file.is_open())
			{
				file << "hello";
				file.close();
			}
			else
			{
                std::cout << "file not opened" << std::endl;
				return ;
			}
}

int main(int argc, char **argv)
{
    //parser
    // try{
    //     Config config("/Users/kbatwoma/Desktop/Webserver_Team/kbatwoma/config/Den/conf/serv_easy.conf");
    //     std::vector<Config::Server> servers = config.getServers();
    //     for (std::vector<Config::Server>::iterator it = servers.begin(); it != servers.end(); it++)
    //     {
    //         std::cout << "IP: " << it->ip << std::endl;
    //         std::cout << "Port: " << it->port << std::endl;
    //         std::cout << "Server name: " << it->server_name << std::endl;
    //         std::cout << "Error page: " << it->error_page << std::endl;
    //         std::cout << "Locations: " << std::endl;
    //         location(it->locations);
    //         // std::cout << "Client body size: " << it->client_body_size << std::endl;
    //         std::cout << std::endl;
    //     }
    // }
    // catch(std::exception &e)
    // {
    //     std::cerr << e.what() << std::endl;
    // }

    // sleep(10000);

    //autoindex

    // (void)argc;
    // std::string str(argv[1]);
    // Autoindex   au(str);
    // std::cout << au.getBody() << std::endl;


    //put
    (void)argc;
    std::string str(argv[1]);
    put(str);
    
    return (0);
}