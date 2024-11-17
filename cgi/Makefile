#cgi単体テスト用Makefile

NAME=cgitest
CXX=c++
CXXFLAGS=-std=c++98 -Wall -Wextra -Werror
SRC=CgiHandler.cpp cgitest_main.cpp ../request/HTTPRequest.cpp
OBJ=$(SRC:.cpp=.o)

test: re
	clear
	@echo "----CGI_TEST----"
	./$(NAME)

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJ)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
