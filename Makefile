# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: melee <melee@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/20 13:12:42 by yetay             #+#    #+#              #
#    Updated: 2023/12/04 12:30:55 by melee            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CPP = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address -g3
RM = rm -rf

NAME = webserv 

SRCS = main.cpp CustomException.cpp HTTP.cpp \
	   Config.cpp Server.cpp Location.cpp \
	   ConfigServer.cpp ConfigLocation.cpp \
       Request.cpp Response.cpp Connection.cpp \
	   Poll.cpp CgiHandler.cpp
OBJS = $(SRCS:%.cpp=obj/%.o)

.PHONY: all \
		test \
		clean fclean re

all: $(NAME)

$(NAME): $(OBJS)
	@$(CPP) $(CFLAGS) -o $@ $^

$(OBJS): obj/%.o: %.cpp | obj
	@$(CC) -I$(INCLUDES) $(CFLAGS) -c -o $@ $<

obj:
	@mkdir -p obj

test: $(NAME)
	@./$(NAME)

clean:
	@$(RM) $(OBJS)
	@$(RM) obj

fclean: clean
	@$(RM) $(NAME)

re: fclean all

