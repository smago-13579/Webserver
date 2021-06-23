# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: smago <smago@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/05/25 14:03:12 by smago             #+#    #+#              #
#    Updated: 2021/06/23 12:46:44 by smago            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserver

SRCS = main.cpp Server.cpp Socket.cpp Request.cpp Response.cpp Config.cpp \
		Cgi_handler.cpp Autoindex.cpp errors.cpp

HEADERS = Autoindex.hpp Cgi_handler.hpp Config.hpp errors.hpp Request.hpp \
		Response.hpp Server.hpp Socket.hpp webserv.hpp

OBJDIR = ./obj/

OBJS = $(SRCS:%.cpp=$(OBJDIR)%.o)

FLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJDIR) $(OBJS) $(HEADERS)
	clang++ $(FLAGS) $(OBJS) -o $(NAME) 

$(OBJDIR)%.o : %.cpp 
	clang++ -c $(FLAGS) $< -o $@

$(OBJDIR):
	mkdir $(OBJDIR)

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all