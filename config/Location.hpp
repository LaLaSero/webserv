/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 16:51:18 by yanagitaryu       #+#    #+#             */
/*   Updated: 2024/12/09 16:47:04 by yutakagi         ###   ########.fr       */
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
    std::pair<int, std::string> getRedirection() const;
    std::string getRootDirectory() const;
    bool isDirectoryListing() const;
    std::string getDefaultFile() const;
    std::map<std::string, std::string> getCgiExtension() const;
    std::string getUploadDirectory() const;
    // Setter
    void setPath(const std::string &path);
    void setAcceptedMethods(const std::vector<std::string> &methods);
    void setRedirection(const std::pair<int, std::string> &redirection);
    void setRootDirectory(const std::string &rootDirectory);
    void setDirectoryListing(bool directoryListing);
    void setDefaultFile(const std::string &defaultFile);
    void setCgiExtension(const std::map<std::string, std::string> &cgiExtension);
    void setUploadDirectory(const std::string &uploadDirectory);


	void add_Acceptedmethods(const std::string tmp);
    Location& operator=(const Location& other);
private:
    std::string path_;
    std::vector<std::string> accepted_methods_;
	std::string root_directory_;
    bool directory_listing_;
    std::pair<int, std::string> redirection_;
    std::string default_file_;
	std::string upload_directory_;
	
    std::map<std::string, std::string> cgi_extension_;
};

#define req_max_body_size 1048576

