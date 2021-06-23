# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kbatwoma <kbatwoma@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/05/25 14:03:12 by smago             #+#    #+#              #
#    Updated: 2021/06/23 12:28:56 by kbatwoma         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserver

SRCS = main.cpp Server.cpp Socket.cpp Request.cpp Response.cpp Config.cpp \
		Cgi_handler.cpp Autoindex.cpp errors.cpp

FLAGS = -Wall -Wextra -Werror

S = -fsanitize=address

all: $(NAME)

$(NAME): $(SRCS)
	clang++ $(SRCS) $(FLAGS) -g -o $(NAME)

clean:
	rm -rf $(NAME) $(NAME).dSYM

re: clean all