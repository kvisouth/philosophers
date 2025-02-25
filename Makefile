SRC		=	src/main.c \
			src/not_libft.c \
			src/parsing.c \

NAME	= philo
CC		= cc
CFLAGS	= -Wall -Wextra -Werror
RM		= rm -rf
AR		= ar rcs

OBJ		= $(SRC:.c=.o)

%.o: %.c
			$(CC) $(CFLAGS) -c $< -o $@

$(NAME):	$(OBJ)
			$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

$(LIBFT):
			$(MAKE) -C

all:		$(NAME)

clean:
			$(RM) $(OBJ)

fclean:		clean
			$(RM) $(NAME)

re:			fclean all

.PHONY:		all clean fclean re
