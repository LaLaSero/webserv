/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yanagitaryusei <yanagitaryusei@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 15:48:46 by yanagitaryu       #+#    #+#             */
/*   Updated: 2024/10/26 15:46:21 by yanagitaryu      ###   ########.fr       */
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

std::pair<std::string, std::string> Location::getRedirection() const {
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

void Location::setRedirection(const std::pair<std::string, std::string> &redirection) 
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