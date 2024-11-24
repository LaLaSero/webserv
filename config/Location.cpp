/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryanagit <ryanagit@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 15:48:46 by yanagitaryu       #+#    #+#             */
/*   Updated: 2024/11/24 11:03:05 by ryanagit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location() :  directory_listing_(false), client_max_body_size_(0) {}

std::string Location::getPath() const {
    return path_;
}

std::vector<std::string> Location::getAcceptedMethods() const {
    return accepted_methods_;
}

std::pair<int, std::string> Location::getRedirection() const {
    return redirection_;
}

std::string Location::getRootDirectory() const {
    return root_directory_;
}

bool Location::isDirectoryListing() const {
    return directory_listing_;
}

std::string Location::getDefaultFile() const {
    return default_file_;
}

std::map<std::string, std::string> Location::getCgiExtension() const {
    return cgi_extension_;
}

std::string Location::getUploadDirectory() const {
    return upload_directory_;
}

size_t Location::getClientMaxBodySize() const {
    return client_max_body_size_;
}



// Setter
void Location::setPath(const std::string &path) {
    path_ = path;
}

void Location::setAcceptedMethods(const std::vector<std::string> &methods) {
    accepted_methods_ = methods;
}

void Location::setRedirection(const std::pair<int, std::string> &redirection) 
{
    redirection_ = redirection;
}

void Location::setRootDirectory(const std::string &rootDirectory) {
    root_directory_ = rootDirectory;
}

void Location::setDirectoryListing(bool directoryListing) {
    directory_listing_ = directoryListing;
}

void Location::setDefaultFile(const std::string &defaultFile) {
    default_file_ = defaultFile;
}

void Location::setCgiExtension(const std::map<std::string, std::string> &cgiExtension) {
    cgi_extension_ = cgiExtension;
}

void Location::setUploadDirectory(const std::string &uploadDirectory) {
    upload_directory_ = uploadDirectory;
}

void Location::setClientMaxBodySize(size_t maxSize) {
    client_max_body_size_ = maxSize;
}

void Location::add_Acceptedmethods(const std::string tmp)
{
	accepted_methods_.push_back(tmp);
}

Location& Location::operator=(const Location& other)
{
    // 自分自身に代入しないようにチェック
    if (this == &other) {
        return *this;  // 同一オブジェクトに対する代入は何もしない
    }

    // メンバー変数のコピー
    path_ = other.path_;
    accepted_methods_ = other.accepted_methods_;
    root_directory_ = other.root_directory_;
    directory_listing_ = other.directory_listing_;
    redirection_ = other.redirection_;
    default_file_ = other.default_file_;
    upload_directory_ = other.upload_directory_;
    client_max_body_size_ = other.client_max_body_size_;
    cgi_extension_ = other.cgi_extension_;

    // コピーしたオブジェクト自身を返す
    return *this;
}
