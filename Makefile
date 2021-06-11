# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: smago <smago@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/05/25 14:03:12 by smago             #+#    #+#              #
#    Updated: 2021/06/11 22:20:07 by smago            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserver

SRCS = main.cpp Server.cpp Socket.cpp Request.cpp kbatwoma/config/Config.cpp

FLAGS = -Wall -Wextra -Werror

S = -fsanitize=address

all: $(NAME)

$(NAME): $(SRCS)
	clang++ $(SRCS) $(S) -g -o $(NAME)

clean:
	rm -rf $(NAME) $(NAME).dSYM

re: clean all