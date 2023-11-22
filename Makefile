# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: melee <melee@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/20 13:12:42 by yetay             #+#    #+#              #
#    Updated: 2023/11/22 17:22:36 by yetay            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CPP = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98
RM = rm -rf

NAME = webserv 

SRCS = main.cpp CustomException.cpp HTTP.cpp \
	   Config.cpp Server.cpp Location.cpp \
       Request.cpp Connection.cpp
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

