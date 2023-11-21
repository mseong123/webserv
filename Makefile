# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: melee <melee@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/20 13:12:42 by yetay             #+#    #+#              #
#    Updated: 2023/11/20 17:13:45 by melee            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CPP = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98
RM = rm -rf

NAME = webserv 

SRCS = main.cpp CustomException.cpp HTTP.cpp Config.cpp
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

