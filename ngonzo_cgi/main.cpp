#include "main.hpp"
#include "cgi-bin/cpp/ft_cgi.hpp"

int		ft_error(std::string message)
{
	PRINT(CLR_RED << message << CLR_END);
	return 127;
}

void		ft_wait()
{
	PRINT("\n10 seconds to complete the test\n...");
	sleep(5);
	for (int i = 5; i != 1; sleep(1))
		PRINT(i-- << " seconds left");
	PRINT("1 second left");
	sleep(1);
}

bool		execute_cgi_script(std::string filename)
{
	PRINT(filename);
	return true;
}


int			main(int argc, char **argv, char **env)
{
	// std::string		file_cpp = "/Users/ngonzo/Desktop/projects/webserv/cgi/cgi-bin/cpp/hello";
	// std::string		file_py = "/Users/ngonzo/Desktop/projects/webserv/cgi/cgi-bin/py/hello.py";
	// std::string		cgi_tester = "/Users/ngonzo/Desktop/projects/webserv/cgi/cgi-bin/cgi_tester";
	std::string		file_cpp = "./cgi-bin/cpp/hello";
	std::string		file_py = "./cgi-bin/py/hello.py";
	std::string		cgi_tester = "./cgi-bin/cgi_tester";
	ft_cgi			test;
	bool			check;

	PRINT_TITLE("cpp")
	ft_hello();

	PRINT_TITLE("execve")
	test.set_filename(file_cpp);
	check = test.execute();
	if(check == false)
		return ft_error("file_cpp not found!");
	
	PRINT_TITLE("python")
	test.set_filename(file_py);
	check = test.execute();
	if(check == false)
		return ft_error("file_py not found!");

	PRINT_TITLE("cgi_tester")
	test.set_filename(cgi_tester);
	check = test.execute();
	if(check == false) 
		return ft_error("cgi_tester not found!");

	ft_wait();
	return 0;
}