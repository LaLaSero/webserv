SRC = main.cpp\
	HTTPRequest.cpp\
	HTTPResponse.cpp\
	HTTPResponseGET.cpp\
	HTTPResponsePOST.cpp\
	HTTPResponseDELETE.cpp\
	HTTPStatusMessage.cpp\
	HandleNormalRequest.cpp\
	HandleAutoIndexRequest.cpp\
	HandleCGIRequest.cpp\
	HandleRedirectRequest.cpp\
	ParseRequest.cpp\
	./parse_state/BodyState.cpp\
	./parse_state/FinishState.cpp\
	./parse_state/HeadersState.cpp\
	./parse_state/RequestLineState.cpp\
	../config/Location.cpp\
	../config/ChildServer.cpp\
	../config/Config.cpp\
	../config/Parser.cpp

OBJ = $(SRC:.cpp=.o)
CXX = c++
RM = rm -f
CXXFLAGS = -std=c++98
# -Wall -Wextra -Werror

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