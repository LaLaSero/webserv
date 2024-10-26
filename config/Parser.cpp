/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yanagitaryusei <yanagitaryusei@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 14:35:31 by yanagitaryu       #+#    #+#             */
/*   Updated: 2024/10/25 20:45:47 by yanagitaryu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"Parser.hpp"
#include"Server.hpp"

Parser::Parser()
{
}

void Parser::LoadFile(const std::string& file_path)
{
    std::ifstream ifs(file_path);
    if (!ifs) 
        throw std::runtime_error("Failed to open file: " + file_path);

    std::stringstream ss;
    ss << ifs.rdbuf();
    content_.str(ss.str());
}


void Parser::ValidIp(std::string &ip) 
{
    std::vector<std::string> parts;
    std::string part;
    std::istringstream ss(ip);

    while (std::getline(ss, part, '.')) {
        parts.push_back(part);
    }

    if (parts.size() != 4) {
        std::cerr << ip << " is invalid: should contain 4 parts." << std::endl;
		throw std::exception();
        return;
    }

    for (std::vector<std::string>::iterator it = parts.begin(); it != parts.end(); ++it) 
	{
        if (it->empty() || it->find_first_not_of("0123456789") != std::string::npos) 
		{
            std::cerr << ip << " is invalid: non-numeric characters found." << std::endl;
			throw std::exception();
        }
        int num = std::atoi(it->c_str());
        if (num < 0 || num > 255) 
		{
            std::cout << ip << " is invalid: each part should be between 0 and 255." << std::endl;
			throw std::exception();
        }
    }
}


void Parser::ValidPort(std::string &port) 
{
    if (port.empty()) 
        throw std::invalid_argument("Port is invalid: empty string provided.");
    for (std::string::iterator it = port.begin(); it != port.end(); ++it) 
	{
        if (!std::isdigit(*it)) 
            throw std::invalid_argument("Port is invalid: contains non-numeric characters.");
    }
    int portNum = std::atoi(port.c_str());
    if (portNum < 0 || portNum > PortMax) 
        throw std::out_of_range("Port is invalid: must be between 0 and 65535.");
}

void Parser::ParseListen(Server &serv, std::string &line)
{
	std::string ip;
	std::string port;

	if (!serv.get_listen_port_().empty())
		throw std::exception();
	if (line[line.size() - 1]!=';' )
		throw std::exception();
	std::string listen_info;
	listen_info = line.substr(line.find(' ')+ 1);
	if (listen_info.find(":") != std::string::npos) 
	{
    	ip = listen_info.substr(0, listen_info.find(":"));
		port = listen_info.substr(listen_info.find(":") + 1);
 	} 
	else 
	{
    	ip = kAnyIpAddress;
		port = listen_info;
	}
	ValidIp(ip);
	ValidPort(port);
	serv.set_listen_ip_(ip);
	serv.set_listen_port_(port);
}

void Parser::ValidServerName(const std::string& str) {
    if (str.empty()) 
	    throw std::runtime_error("error empty:" + str);

    if (str.front() == '.' || str.back() == '.') 
		throw std::runtime_error("error dot in edge:" + str);
    for (size_t i = 0; i < str.length(); ++i)
	{
        char current = str[i];
        if (current == '.' && (str[i - 1] == '.')) 
            throw std::runtime_error("error Consecutive dots:" + str);
        if (!std::isalnum(current) && current != '.') 
            throw std::runtime_error("error found non alnum:" + str);
    }
}

void Parser::ParseServname(Server &serv, std::string &line)
{
	std::string body;
	std::string name;
	body = line.substr(line.find(' ')+ 1);
	if (body.find(';') != body.size() - 1)
		throw std::exception();
	while(body.find(' ') != std::string::npos)
	{
		name = body.substr(0, body.find(' '));
		ValidServerName(name);
		serv.add_server_names_(name);
		body = body.substr(body.find(' ') + 1, body.size());
	}
	name = body.substr(0,body.size() - 1);
	ValidServerName(name);
	serv.add_server_names_(name);
}

void Parser::ValidErrorstatus(std::string &status)
{
	if (status.size() != 3)
		throw std::runtime_error("error Errorpage status:" + status);
	if (!('0' < status[0] &&  status[0] < '6' && isdigit(status[1])&& isdigit(status[2])))
		throw std::runtime_error("error Errorpage status:" + status);
}


void Parser::ParseErrorpage(Server &serv, std::string &line)
{

	std::vector<int> status_vec;

	std::string body;
	std::string status;
	body = line.substr(line.find(' ')+ 1);
	if (body.find(';') != body.size() - 1)
		throw std::runtime_error("ParseErrorpage: not found ';'");
	while(body.find(' ') != std::string::npos)
	{
		status = body.substr(0, body.find(' '));
		ValidErrorstatus(status);
		int i;
		i = (status[0] - '0') * 100 + (status[1] - '0') * 10 + (status[2] - '0');
		if (std::find(status_vec.begin(), status_vec.end(), i) != status_vec.end())
			throw std::runtime_error("ParseErrorpage: multiple status:" + status);
		else
			status_vec.push_back(i);
		body = body.substr(body.find(' ') + 1, body.size());
	}
	std::string page;
	page = body.substr(0,body.size() - 1);
	if (status_vec.empty())
		throw std::runtime_error("ParseErrorpage status empty");
	int i;
	i = 0;
	while (i < status_vec.size())
	{
		serv.add_error_page_(status_vec[i], page);
		i++;
	}
}

size_t Parser::max_stos(std::string &limit)
{
	size_t i;
	size_t count;
	
	i = 0;
	count = 0;
	while(i < limit.size() && i < DEFAULT_RBSIZE)
	{
		if (!(isdigit(limit[i])))
			throw std::runtime_error("ParseCLMAX: Non digit");
		count = count * 10 + count + (limit[i] - '0');
		i++;
	}
	if (i != limit.size())
		throw std::runtime_error("ParseCLMAX: Non digit");
	return (count);
}

void Parser::ParseCLMAX(Server &serv, std::string &line)
{
	if (line[line.size() - 1] != ';')
		throw std::runtime_error("ParseCLMAX: not found ';'");
	std::string limit;
	limit = line.substr(line.find(' ')+ 1);
	limit.pop_back();
	serv.set_request_max(max_stos(limit));
}

void Parser::ParseRewrite(Location &loc, std::string &line)
{
	std::string original;
	std::string late;

	line = line.substr(line.find(' ')+ 1);
	std::cout << line << std::endl;
	if (line.find(' ') == std::string::npos)
		throw std::runtime_error("ParserRewrite Error: '  ' is not found" + line);
	original = line.substr(0, line.find(' '));
	late = line.substr(line.find(' ') + 1);
	late.pop_back();
	std::pair<std::string, std::string> pair(original, late);
	loc.setRedirection(pair);
}


void Parser::ParseLocation(Server &server, std::string &line)
{
	Location loc;

	std::string path;
	path = line.substr(line.find(' ') + 1);
	loc.setPath(path);
	std::string red;
	if (!std::getline(content_, red))
		throw std::runtime_error("ParseLocation Error:'{}' is not found ");
	if (red != "{")
		throw std::runtime_error("ParseLocation Error:'{' is not found" + red);
	while (std::getline(content_, red) && red !=  "}")
	{
		if (red[red.size() - 1] !=';')
			throw std::runtime_error("ParseLocation Error:';' is not found " + red);
		std::string tmp;
		if (red.size() > 6 && red.substr(0,5) == "root ")
		{
			tmp = red.substr(red.find(' ') + 1);
			tmp.erase(tmp.size() - 1);
			loc.setRootDirectory(tmp);
		}
		else if (red.size() > 11 && red.substr(0,10) == "ok_method ")
		{
			tmp = red.substr(red.find(' ') + 1);
			tmp.erase(tmp.size() - 1);
			loc.add_Acceptedmethods(tmp);
		}
		else if (red == "autoindex on;")
			loc.setDirectoryListing(true);
		else if (red == "autoindex off;")
			loc.setDirectoryListing(false);
		else if (red.size() > std::strlen("rewrite ") && red.substr(0,std::strlen("rewrite ")) == "rewrite ")
			ParseRewrite(loc, red);	
		else if (red.size() > std::strlen("index ") && red.substr(0,std::strlen("index ")) == "index ")
		{
			tmp.erase(tmp.size() - 1);
			loc.setDefaultFile(tmp);
		}
		else if  (red.size() > std::strlen("upload_directory ") && red.substr(0,std::strlen("upload_directory ")) == "upload_directory ")
		{
			tmp = red.substr(red.find(' ') + 1);
			tmp.erase(tmp.size() - 1);
			loc.setUploadDirectory(tmp);
		}
		else if (red.size() > std::strlen("client_max ") && red.substr(0,std::strlen("client_max ")) == "client_max ")
		{
			tmp = red.substr(red.find(' ') + 1);
			tmp.erase(tmp.size() - 1);
			if (tmp.size() > 7)
				throw  std::runtime_error("ParseLocation Error:';' too large client_max " + red);
			std::stringstream ss(tmp);
			size_t i;
			ss >> i;
			loc.setClientMaxBodySize(i);
		}
		else if ()
		else
			std::runtime_error("ParseLocation Error:unkown word is found");
	}
	if (red != "}")
		throw std::runtime_error("ParseLocation Error:'}' is not found ");
}

void Parser::MakeServer(Config &conf)
{
	Server serv;
	std::string line;

	if (!(std::getline(content_, line) && line =="{"))
		throw std::exception();
	while(std::getline(content_, line) && line !=  "}")
	{
		if (line.size() >= 8 && line.substr(0, 7) == "listen ")
			ParseListen(serv, line);
		else if (line.size() >= 13 && line.substr(0, 12) == "server_name ")
			ParseServname(serv, line);
		else if (line.size() >= 12 && line.substr(0, 11) == "error_page ")
			ParseErrorpage(serv, line);
		else if (line.size() >= 22 && line.substr(0, 21) == "client_max_body_size ")
			ParseCLMAX(serv, line);
		else if (line.size() >= 10 && line.substr(0, 9) == "location ")
			ParseLocation(serv, line);	
		else
			throw std::runtime_error("found error in server: " + line);
	}
	if (line != "}")
		throw std::exception();
}

Config Parser::MakeConfig()
{
	Config conf;
	std::string line;

	while (std::getline(content_, line)) 
	{
		if (line == "server")
			MakeServer(conf);
		else
			throw std::runtime_error("Error found: " + line +"!");
	}
	return (conf);
}


