NAME = libft.a
SRCS = ft_strlen.c ft_bzero.c ft_memcpy.c ft_strncmp.c ft_toupper.c ft_tolower.c ft_substr.c ft_strdup.c
SRCS += ft_strjoin.c ft_strtrim.c ft_split.c ft_itoa.c ft_strmapi.c ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c
SRCS += ft_putnbr_fd.c ft_atoi.c ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c ft_memset.c
SRCS += ft_memchr.c ft_strchr.c ft_strrchr.c ft_calloc.c ft_memmove.c
SRCS += ft_memcmp.c ft_strlcpy.c ft_strlcat.c ft_strnstr.c ft_striteri.c
BONUS_SRCS = ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c ft_lstadd_back.c
BONUS_SRCS += ft_lstdelone.c ft_lstclear.c ft_lstiter.c ft_lstmap.c
INCLUDE = .
CC = cc
CFLAGS = -Wall -Wextra -Werror
ifdef BONUS
	TARGET_SRCS = $(BONUS_SRCS)
	OBJS = $(BONUS_SRCS:.c=.o)
else
	TARGET_SRCS = $(SRCS)
	OBJS = $(SRCS:.c=.o)
endif
OBJS = $(TARGET_SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

clean:
	rm -f $(SRCS:.c=.o) $(BONUS_SRCS:.c=.o)

fclean: clean
	rm -f $(NAME)

re:
	$(MAKE) fclean
	$(MAKE) all

bonus:
	$(MAKE) all BONUS=1

.PHONY: all clean fclean re bonus