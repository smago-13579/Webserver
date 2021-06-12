# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: smago <smago@student.21-school.ru>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/05/25 14:03:12 by smago             #+#    #+#              #
#    Updated: 2021/06/12 20:00:59 by smago            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserver

SRCS = main.cpp Server.cpp Socket.cpp Request.cpp Response.cpp kbatwoma/config/Config.cpp

FLAGS = -Wall -Wextra -Werror

S = -fsanitize=address

all: $(NAME)

$(NAME): $(SRCS)
	clang++ $(SRCS) -g -o $(NAME)

clean:
	rm -rf $(NAME) $(NAME).dSYM

re: clean all