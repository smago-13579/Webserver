#!/usr/local/bin/python3

print("Content-type: text/html")
print()
print("<h1>Hello world!</h1>")

if	str[0] == str[1] == str[2]\
	or str[3] == str[4] == str[5]\
	or str[6] == str[7] == str[8]\
	or str[0] == str[3] == str[6]\
	or str[1] == str[4] == str[7]\
	or str[2] == str[5] == str[8]\
	or str[0] == str[4] == str[8]\
	or str[2] == str[4] == str[6]:
	print("Winner is " , str[9])
else:
	print("Draw")
 
AUTH_TYPE=Anonymous
CONTENT_LENGTH=100000
CONTENT_TYPE=test/file
GATEWAY_INTERFACE=CGI/1.1
PATH_INFO=/directory
PATH_TRANSLATED=/Users/anatashi/goinfre/webServ/webServ/YoupiBanane/directory
QUERY_STRING=
REMOTE_ADDR=127.0.0.1
REMOTE_IDENT=.localhost:8080
REMOTE_USER=
REQUEST_METHOD=POST
REQUEST_URI=/directory
SCRIPT_NAME=cgi_tester
SERVER_NAME=for_tester
SERVER_PORT=8080
SERVER_PROTOCOL=HTTP/1.1
SERVER_SOFTWARE=webServ
HTTP_accept-encoding=gzip
HTTP_content-type=test/file
HTTP_host=localhost:8080
HTTP_transfer-encoding=chunked
HTTP_user-agent=go-http-client/1.1
HTTP_x-secret-header-for-test=1