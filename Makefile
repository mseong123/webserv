# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yetay <yetay@student.42kl.edu.my>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/20 13:12:42 by yetay             #+#    #+#              #
#    Updated: 2023/11/20 13:13:35 by yetay            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CPP = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98
RM = rm -rf

NAME = webserv

SRCS = src/webserv.cpp
OBJS = $(SRCS:src/%.cpp=obj/%.o)

.PHONY: all \
		test \
		clean fclean re

all: $(NAME)

$(NAME): $(OBJS)
	@$(CPP) $(CFLAGS) -o $@ $^

$(OBJS): obj/%.o: src/%.cpp | obj
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

