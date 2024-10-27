# コンパイラとフラグ
CXX = c++
# CXXFLAGS = -Wall -Wextra -Werror

# 出力ファイル名
NAME = test

# ソースファイルとオブジェクトファイル
SRCS = main.cpp Config.cpp ChildServer.cpp Parser.cpp Location.cpp ../server/EpollAdm.cpp ../server/SocketAddress.cpp ../server/Socket.cpp ../server/ListenSocket.cpp ../server/ConnSocket.cpp
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
