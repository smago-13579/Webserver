# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ngonzo <ngonzo@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/05/25 14:03:12 by smago             #+#    #+#              #
#    Updated: 2021/06/16 13:08:41 by ngonzo           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserver

SRCS = main.cpp Server.cpp Socket.cpp Request.cpp Response.cpp Config.cpp ngonzo/cgi_handler.cpp

FLAGS = -Wall -Wextra -Werror

S = -fsanitize=address

all: $(NAME)

$(NAME): $(SRCS)
	clang++ $(SRCS) -g -o $(NAME) 

clean:
	rm -rf $(NAME) $(NAME).dSYM

re: clean all