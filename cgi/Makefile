#cgi単体テスト用Makefile

NAME=cgitest
CXX=c++
CXXFLAGS= -Wall -Wextra -Werror #-std=c++98
SRC=CgiHandler.cpp\
	cgitest_main.cpp\
	../request/HTTPRequest.cpp\
	  ../config/Location.cpp\
	 CgiResponse.cpp\
	 ../request/ParseRequest.cpp\
	../request/parse_state/RequestLineState.cpp\
	 ../request/parse_state/HeadersState.cpp\
	 ../request/parse_state/BodyState.cpp\
	 ../request/parse_state/FinishState.cpp

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
