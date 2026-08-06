# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vinda-si <vinda-si@student.42sp.org.br>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/08/05 21:44:02 by vinda-si          #+#    #+#              #
#    Updated: 2026/08/05 21:48:59 by vinda-si         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= btc
CXX			= c++
CXXFLAGS	= -Wall -Wextra -Werror -std=c++98

SRCS		= main.cpp BitcoinExchange.cpp
OBJS		= $(SRCS:.cpp=.o)

all:	$(NAME)

$(NAME):	$(OBJS)
		$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

clean:
		rm -f $(OBJS)

fclean:	clean
		rm -f $(NAME)

re:	fclean all

.PHONY:	all fclean clean re
