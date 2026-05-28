# webserv

C++98 で実装した HTTP/1.1 サーバーです。設定ファイルから複数の server / location を読み込み、`epoll` ベースのイベントループで request を少しずつ処理します。

## Build

```sh
make
```

## Usage

```sh
./webserv [config_file]
```

`config_file` を省略した場合は `test.conf` を読み込みます。

## 主な機能

- HTTP/1.1 request parsing
- `GET`, `POST`, `DELETE`
- non-blocking socket + `epoll`
- virtual server
- location ごとの `root`, `index`, `autoindex`, `rewrite`, `upload_directory`
- `client_max_body_size`
- custom error page
- Python CGI
- MIME type の簡易判定

## 実装のポイント

### 非同期 I/O

listen socket、client socket、CGI の pipe を `epoll` に登録し、1つの event loop で監視します。

処理の流れ:

1. `main.cpp` が config を読み込む
2. `set_up_server()` が listen socket を作成する
3. `Loop()` が timeout と I/O event を確認する
4. event handler が accept / read / write / CGI read を処理する

関連ファイル:

- `server/EpollAdm.cpp`
- `server/functions.cpp`
- `server/ListenSocket.cpp`
- `server/ClientSocket.cpp`

### 少しずつ進む request parser

request は一度で全て届くとは限らないため、client socket の受信 buffer に読めた分だけ追加します。

parser は state pattern で実装しています。

- `RequestLineState`
- `HeadersState`
- `BodyState`
- `FinishState`

`ParseRequest::parse()` は現在の state を実行し、データが足りない場合は同じ state に留まります。これにより、request line、headers、body を段階的に処理できます。

関連ファイル:

- `request/ParseRequest.cpp`
- `request/parse_state/`

### HTTP status code

HTTP status code は `request/HTTPStatusCode.hpp` に enum としてまとめ、`request/HTTPStatusMessage.cpp` で reason phrase に変換します。

RFC 9110 / RFC 9112 の HTTP 仕様にある status code を参照し、1xx から 5xx までの代表的な code を表として持たせています。response の status line や error response はこの定義を使って生成します。

主に使っている error:

- `400 Bad Request`
- `403 Forbidden`
- `404 Not Found`
- `405 Method Not Allowed`
- `413 Payload Too Large`
- `500 Internal Server Error`
- `501 Not Implemented`
- `505 HTTP Version Not Supported`

関連ファイル:

- `request/HTTPStatusCode.hpp`
- `request/HTTPStatusMessage.cpp`
- `server/ServerException.hpp`
- `request/HTTPResponseError.cpp`

## Config

設定ファイルは `server` block を並べて書きます。

```conf
server
{
    listen 8081;
    server_name hello.a world end;
    client_max_body_size 1000;
    error_page 404 /path/to/404.html;

    location /docs
    {
        root /path/to/root/;
        autoindex on;
        ok_method GET;
    }

    location /docs/upload
    {
        root /path/to/root/;
        autoindex on;
        ok_method GET;
        ok_method POST;
        ok_method DELETE;
        upload_directory /path/to/upload;
    }

    location /docs/redirect
    {
        ok_method GET;
        rewrite 301 /docs/index;
        index index.html;
        autoindex off;
    }
}
```

対応 directive:

- `listen`
- `server_name`
- `error_page`
- `client_max_body_size`
- `location`
- `root`
- `ok_method`
- `autoindex`
- `rewrite`
- `index`
- `upload_directory`

## Request handling

### GET

`root` と request URI からファイルを探して返します。directory の場合は `index` または `autoindex` の設定に従います。

### POST

body size が `client_max_body_size` を超えた場合は `413 Payload Too Large` を返します。upload directory が設定されている場合は `posted.txt` として保存します。

### DELETE

対象ファイルを削除します。directory に対する DELETE は `400 Bad Request` として扱います。

### CGI

URI に `/cgi-bin/` を含む request は CGI として処理します。CGI process の stdout を pipe 経由で読み取り、その pipe も `epoll` に登録して response に変換します。

関連ファイル:

- `cgi/CgiHandler.cpp`
- `cgi/CgiResponse.cpp`
- `server/functions.cpp`

## Directory

```text
.
|-- config/      # config parser and server/location model
|-- server/      # socket, epoll, event loop
|-- request/     # HTTP request parser and response builder
|-- cgi/         # CGI execution and response parsing
|-- docs/        # sample document root
|-- cgi-bin/     # sample CGI scripts
|-- test.conf    # sample config
`-- Makefile
```

## Clean

```sh
make clean
make fclean
make re
```
