Configの設定について

Config
では、
server
{
listen 127.0.0.1:8080;
server_name hello.a world end;
error_page 404 123;
client_max_body_size 1;
}
server
{
server_name hello.a world end;
error_page 404 123;
client_max_body_size 1;
location hello
{
root hello;
rewrite a b;
}
}

となるようにしている
一番外がserver
その中(以降ブロック、サーバーブロック)には、
listen
server_name
error_page
client_max_body_size
location
がおける

locationブロックの中には、
root
rewerite
index
upload_directory
autoindex
ok_method
が定義できるようになっている


現状、余計な空白および改行は不可


