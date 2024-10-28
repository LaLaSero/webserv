SRC = main.cpp\
	HTTPRequest.cpp\
	ParseRequest.cpp\
	./parse_stage/BodyState.cpp\
	./parse_stage/FinishState.cpp\
	./parse_stage/HeadersState.cpp\
	./parse_stage/RequestLineState.cpp\

OBJ = $(SRC:.cpp=.o)
CXX = c++
RM = rm -f
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

NAME = webserv

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean $(NAME)

.PHONY: all clean fclean re