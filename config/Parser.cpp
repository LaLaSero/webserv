/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryanagit <ryanagit@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 14:35:31 by yanagitaryu       #+#    #+#             */
/*   Updated: 2024/11/26 17:59:37 by ryanagit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"Parser.hpp"
#include"ChildServer.hpp"

Parser::Parser()
{
}

Parser::~Parser()
{
}

Parser::Parser(const Parser &other)
{
	*this = other;
}

Parser& Parser::operator=(const Parser& other)
    {
        if (this != &other)
        {
            content_.str(other.content_.str());
            content_.clear();
        }
        return (*this);
    }


void Parser::LoadFile(const std::string& file_path)
{
    // std::ifstreamのオープン
    std::ifstream ifs(file_path.c_str());
    if (!ifs) {
        throw std::runtime_error("Failed to open file: " + file_path);
    }

    // ストリームに内容を読み込む
    std::stringstream ss;
    ss << ifs.rdbuf();

    // 文字列ストリームの内容をcontent_にセット
    content_.str(ss.str());
}


void Parser::ValidIp(std::string &ip) 
{
    std::vector<std::string> parts;
    std::string part;
    std::istringstream ss(ip);

    while (std::getline(ss, part, '.')) 
	{
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
            std::cerr << ip << " is invalid: each part should be between 0 and 255." << std::endl;
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
            throw std::invalid_argument("Port is invalid: contains non-numeric characters:" + port);
    }
    int portNum = std::atoi(port.c_str());
    if (portNum < 0 || portNum > PortMax) 
        throw std::out_of_range("Port is invalid: must be between 0 and 65535.");
}

void Parser::ParseListen(ChildServer &serv, std::string &line)
{
	std::string ip;
	std::string port;

	std::string listen_info;
	listen_info = line.substr(line.find(' ')+ 1);
	if (listen_info.find(":") != std::string::npos) 
	{
    	ip = listen_info.substr(0, listen_info.find(":"));
		port = listen_info.substr(listen_info.find(":") + 1);
		port.erase(port.size() - 1);
 	} 
	else 
	{
    	ip = kAnyIpAddress;
		port = listen_info;
		port.erase(port.size() - 1);
	}
	ValidIp(ip);
	ValidPort(port);
	serv.set_listen_ip_(ip);
	serv.set_listen_port_(port);
}

void Parser::ValidChildServerName(const std::string& str) {
    if (str.empty()) 
	    throw std::runtime_error("error empty:" + str);
    if (str[0] == '.' || str[str.size() - 1] == '.') 
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

void Parser::ParseServname(ChildServer &serv, std::string &line)
{
	std::string body;
	std::string name;
	body = line.substr(line.find(' ')+ 1);
	if (body.find(';') != body.size() - 1)
		throw std::exception();
	while(body.find(' ') != std::string::npos)
	{
		name = body.substr(0, body.find(' '));
		ValidChildServerName(name);
		serv.add_server_names_(name);
		body = body.substr(body.find(' ') + 1, body.size());
	}
	name = body.substr(0,body.size() - 1);
	ValidChildServerName(name);
	serv.add_server_names_(name);
}

void Parser::ValidErrorstatus(std::string &status)
{
	if (status.size() != 3)
		throw std::runtime_error("error Errorpage status:" + status);
	if (!('0' < status[0] &&  status[0] < '6' && isdigit(status[1])&& isdigit(status[2])))
		throw std::runtime_error("error Errorpage status:" + status);
}


void Parser::ParseErrorpage(ChildServer &serv, std::string &line)
{

	std::vector<int> status_vec;

	std::string body;
	std::string status;
	body = line.substr(line.find(' ')+ 1);
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
	page = body.substr(0);
	page.erase(page.size() - 1);
	if (status_vec.empty())
		throw std::runtime_error("ParseErrorpage status empty");
	unsigned long i;
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
		count = count * 10 + (limit[i] - '0');
		i++;
	}
	if (i != limit.size())
		throw std::runtime_error("ParseCLMAX: Non digit");
	return (count);
}

void Parser::ParseCLMAX(ChildServer &serv, std::string &line)
{
	std::string limit;
	limit = line.substr(line.find(' ')+ 1);
	limit.erase(limit.size() - 1);;
	serv.set_request_max(max_stos(limit));
}

void Parser::ParseRewrite(Location &loc, std::string &line)
{
	std::string original;
	std::string late;
	size_t i;
	line = line.substr(line.find(' ')+ 1);
	original = line.substr(0, line.find(' '));
	late = line.substr(line.find(' ') + 1);
	late.erase(late.size() - 1);
	i = max_stos(original);
	if (i < 301 || (303 < i && i < 307) || 308 < i)
		throw std::runtime_error("ParseCLMAX: Wrong Number Redirect");
	std::pair<int, std::string> pair(i, late);
	loc.setRedirection(pair);
}

static std::string EmitEmpty(std::string &original)
{
	size_t i =0;
	while(original[i] ==' ')
		i++;
	return (original.substr(i));
}


void Parser::ParseLocation(ChildServer &server, std::string &line)
{
	Location loc;

	std::string path;
	path = line.substr(line.find(' ') + 1);
	loc.setPath(path);
	std::string red;
	if (!std::getline(content_, red))
		throw std::runtime_error("ParseLocation Error:'{}' is not found ");
	red = EmitEmpty(red);
	if (red != "{")
		throw std::runtime_error("ParseLocation Error:'{' is not found" + red);
	while (std::getline(content_, red))
	{
		red = EmitEmpty(red);
		if (red == "}")
			break ;
		std::string tmp;
		if (check_syntax(red, "root ",true))
		{
			tmp = red.substr(red.find(' ') + 1);
			tmp.erase(tmp.size() - 1);
			loc.setRootDirectory(tmp);
		}
		else if (check_syntax(red, "ok_method ",true))
		{
			tmp = red.substr(red.find(' ') + 1);
			tmp.erase(tmp.size() - 1);
			loc.add_Acceptedmethods(tmp);
		}
		else if (red == "autoindex on;")
			loc.setDirectoryListing(true);
		else if (red == "autoindex off;")
			loc.setDirectoryListing(false);
		else if (check_syntax(red, "rewrite ",true))
			ParseRewrite(loc, red);	
		else if (check_syntax(red, "index ",true))
		{
			tmp = red.substr(red.find(' ') + 1);
			tmp.erase(tmp.size() - 1);
			loc.setDefaultFile(tmp);
		}
		else if ( check_syntax(red, "upload_directory ",true))
		{
			tmp = red.substr(red.find(' ') + 1);
			tmp.erase(tmp.size() - 1);
			loc.setUploadDirectory(tmp);
		}
		else
			throw std::runtime_error("ParseLocation Error:unkown word is found");
		// we have to add cgi infomation;
		// else if ()
	}
	if (red != "}")
		throw std::runtime_error("ParseLocation Error:'}' is not found ");
	server.add_location(loc);
}

void Parser::MakeChildServer(Config &conf)
{
	ChildServer serv;
	std::string line;

	serv.set_request_max(req_max_body_size);
	if (!(std::getline(content_, line) && line =="{"))
		throw std::exception();
	while(std::getline(content_, line) && line !=  "}")
	{
		line = EmitEmpty(line);
		if (check_syntax(line, "listen ",true))
			ParseListen(serv, line);
		else if (check_syntax(line, "server_name ", true))
			ParseServname(serv, line);
		else if (check_syntax(line, "error_page ", true))
			ParseErrorpage(serv, line);
		else if (check_syntax(line, "client_max_body_size ", true))
			ParseCLMAX(serv, line);
		else if (check_syntax(line, "location ", false))
			ParseLocation(serv, line);	
		else
			throw std::runtime_error("found error in server: '" + line + "'");
	}
	if (line != "}")
		throw std::exception();
	conf.addChildServer(serv);
}

Config Parser::MakeConfig()
{
	Config conf;
	std::string line;

	while (std::getline(content_, line)) 
	{
		if (line == "server")
			MakeChildServer(conf);
		else
			throw std::runtime_error("Error found: " + line +"!");
	}
	return (conf);
}

bool Parser::check_syntax(std::string &line, std::string target,bool flag)
{
	if (line.size() > target.size() + 1 && line.substr(0, target.size()) == target)
	{
		if (flag)
		{
		 if(line[line.size() - 1] == ';')
			return (true);
		 else
		 	return (false);
		}
		else
			return (true);
	}
	else
		return (false);
}


