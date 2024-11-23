/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryanagit <ryanagit@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 16:51:18 by yanagitaryu       #+#    #+#             */
/*   Updated: 2024/11/23 16:42:38 by ryanagit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <vector>
#include <map>
#include <cstring>
#include <unistd.h>
#include <algorithm>


class Location 
{
public:
    Location();

    // Getter
    std::string getPath() const;
    std::vector<std::string> getAcceptedMethods() const;
    std::pair<std::string, std::string> getRedirection() const;
    std::string getRootDirectory() const;
    bool isDirectoryListing() const;
    std::string getDefaultFile() const;
    std::map<std::string, std::string> getCgiExtension() const;
    std::string getUploadDirectory() const;
    size_t getClientMaxBodySize() const;

    // Setter
    void setPath(const std::string &path);
    void setAcceptedMethods(const std::vector<std::string> &methods);
    void setRedirection(const std::pair<std::string, std::string> &redirection);
    void setRootDirectory(const std::string &rootDirectory);
    void setDirectoryListing(bool directoryListing);
    void setDefaultFile(const std::string &defaultFile);
    void setCgiExtension(const std::map<std::string, std::string> &cgiExtension);
    void setUploadDirectory(const std::string &uploadDirectory);
    void setClientMaxBodySize(size_t maxSize);


	void add_Acceptedmethods(const std::string tmp);
    Location& operator=(const Location& other);
private:
    std::string path_;
    std::vector<std::string> accepted_methods_;
	std::string root_directory_;
    bool directory_listing_;
    std::pair<std::string, std::string> redirection_;
    std::string default_file_;
	std::string upload_directory_;
    size_t client_max_body_size_;
	
    std::map<std::string, std::string> cgi_extension_;
};

