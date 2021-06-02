# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: smago <smago@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/05/25 14:03:12 by smago             #+#    #+#              #
#    Updated: 2021/06/02 16:59:59 by smago            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserver

SRCS = main.cpp Server.cpp 

FLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(SRCS) $(CLS)
	clang++ $(SRCS) -o $(NAME)

clean:
	rm -rf $(NAME)

re: clean all