/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MethodUtils.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryanagit <ryanagit@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 20:00:39 by ryanagit          #+#    #+#             */
/*   Updated: 2024/11/26 12:46:25 by ryanagit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include"HTTPRequest.hpp"
#include"HTTPResponse.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <cstdio>
#include<sys/stat.h>
#include<map>


class ChildServer; // Forward declaration of ChildServer class if needed
class Location; // Forward declaration of Location class if needed

// HTTPレスポンスで使用される関数群のプロトタイプ宣言

// ファイルの拡張子を取得
std::string get_file_extension(std::string& file_path);

// ファイルのコンテンツを取得
std::string get_file_content(const std::string& path);

// リダイレクト用のHTMLボディを生成
std::string get_redirect_body(const std::string uri);

// 403 Forbidden エラーページを取得
std::string get_error_page(int status, const ChildServer *serv, std::string default_message);

// 実際のパスを生成
std::string make_true_path(const std::string &uri, const std::string &root_path);


// サーバー上のパスがディレクトリかどうかを判定
bool is_dir(const std::string &path);

// `Location` に基づいて適切なロケーションを探す
std::vector<Location>::const_iterator find_location(const ChildServer *Server, const std::string& path);


#define P200 "<html><head><title>400 Bad Request</title></head><body><h1>400 Bad Request</h1></body></html>"

#define EP400 "<html><head><title>400 Bad Request</title></head><body><h1>400 Bad Request</h1></body></html>"
#define EP403 "<html><head><title>403 Forbidden</title></head><body><h1>403 Forbidden</h1></body></html>"
#define EP404  "<html><head><title>404 Not Found</title></head><body><h1>404 Not Found</h1><p>The requested URL was not found on this server.</p></body></html>"
#define EP500  "<html><head><title>500 Internal Server </title></head><body><h1>500 Internal Server </h1></body></html>"