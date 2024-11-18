SRC = main.cpp\
	HTTPRequest.cpp\
	HTTPResponse.cpp\
	HTTPResponseGET.cpp\
	HTTPResponsePOST.cpp\
	HTTPResponseDELETE.cpp\
	HTTPStatusMessage.cpp\
	ParseRequest.cpp\
	./parse_state/BodyState.cpp\
	./parse_state/FinishState.cpp\
	./parse_state/HeadersState.cpp\
	./parse_state/RequestLineState.cpp

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