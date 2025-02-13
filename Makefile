# コンパイラとフラグ
CXX = c++
CXXFLAGS =  -std=c++98 -Wall -Wextra -Werror -g
# 出力ファイル名
NAME = webserv

# ソースファイルとオブジェクトファイル
SRCS = main.cpp \
       config/Config.cpp \
       config/ChildServer.cpp \
       config/Parser.cpp \
       config/Location.cpp \
       server/EpollAdm.cpp \
       server/SocketAddress.cpp \
       server/Socket.cpp \
       server/ListenSocket.cpp \
       server/ClientSocket.cpp \
       server/functions.cpp \
       server/ServerException.cpp \
       request/HandleAutoIndexRequest.cpp \
       request/HandleCGIRequest.cpp \
       request/HandleNormalRequest.cpp \
       request/HTTPRequest.cpp \
       request/HTTPResponseDELETE.cpp \
       request/HTTPResponseGET.cpp \
       request/HTTPResponsePOST.cpp \
       request/HTTPResponseError.cpp \
       request/ParseRequest.cpp \
       request/HTTPStatusMessage.cpp \
       request/HTTPResponse.cpp\
       request/MethodUtils.cpp\
       request/parse_state/RequestLineState.cpp \
       request/parse_state/HeadersState.cpp \
       request/parse_state/FinishState.cpp \
       request/parse_state/BodyState.cpp \
       cgi/CgiHandler.cpp \
       cgi/CgiResponse.cpp

OBJS = $(SRCS:.cpp=.o)

# デフォルトターゲット
all: $(NAME)

# リンクして実行ファイルを作成
$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

# .cppファイルから.oファイルを生成
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# クリーンアップ
clean:
	rm -rf $(OBJS)

# 実行ファイルを含む全ての生成物を削除
fclean: clean
	rm -rf $(NAME)

# リビルド
re: fclean all

# .PHONYターゲット
.PHONY: all clean fclean re