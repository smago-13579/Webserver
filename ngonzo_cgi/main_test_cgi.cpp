#include "main_test_cgi.hpp"
#include "cgi_handler.hpp"

// class Response {};	// test
// class Request {};	// test

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

// void	_post_request_handle(Response & resp, Request & req)
// {
// 	std::string		file_path;

// 	if(req.get_body().len() > _location.max_body)
// 	{
// 		resp.body = "";
// 		resp.status_code_int_val = 433;
// 		resp.reason_phrase = "Payload Too Large";
// 		resp.add_header("Content-Length", "0");
// 	}
// 	else if(req.get_body().len() == _location.max_body or
// 		(req.get_body().len() == 0 and req.get_query_string().empty()))
// 	{
// 		resp.body = req.get_body();
// 		resp.status_code_int_val = 200;
// 		resp.reason_phrase = "OK";
// 		resp.add_header("Content-Length", to_string(resp.body.len()));
// 	}
// 	else
// 	{
// 		cgi_handler cgi;
// 		cgi.set_param(req, file_path);
// 		if(cgi)
// 		{
// 			bool	check = cgi.execute();
// 			if(check == true)
// 			{
// 				resp.body = cgi.get_response_body();
// 				resp.status_code_int_val = cgi.get_status_code();
// 				resp.add_header("Content-Type", cgi.get_str_content_type().cubstr(14));
// 				resp.reason_phrase = cgi.get_str_status_code();
// 				resp.add_header("Content-Length", to_string(cgi.get_response_body().size()));
// 			}
// 			else
// 				resp = ERROR;
// 		}
// 		else
// 			resp = ERROR;
// 	}
// }


int			main(int argc, char **argv, char **env)
{
	// std::string		file_cpp = "/Users/ngonzo/Desktop/projects/webserv/cgi/cgi-bin/cpp/hello";
	// std::string		file_py = "/Users/ngonzo/Desktop/projects/webserv/cgi/cgi-bin/py/hello.py";
	// std::string		cgi_tester = "/Users/ngonzo/Desktop/projects/webserv/cgi/cgi-bin/cgi_tester";
	std::string		file_cpp = "./cgi-bin/cpp/hello";
	std::string		file_py = "./cgi-bin/py/hello.py";
	std::string		cgi_tester = "./cgi-bin/cgi_tester";
	cgi_handler		cgi;
	bool			check;

	PRINT_TITLE("cpp")
	ft_hello();

	PRINT_TITLE("execve")
	cgi.set_filename(file_cpp);
	check = cgi.execute();
	if(check == false)
		ft_error("file_cpp not found!\n");
	
	PRINT_TITLE("python")
	cgi.set_filename(file_py);
	check = cgi.execute();
	if(check == false)
		ft_error("file_py not found!\n");

	PRINT_TITLE("cgi_tester")
	cgi.set_filename(cgi_tester);
	check = cgi.execute();
	if(check == false) 
		ft_error("cgi_tester not found!\n");

	ft_wait();
	return 0;
}