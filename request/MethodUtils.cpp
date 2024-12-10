/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MethodUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 20:00:43 by ryanagit          #+#    #+#             */
/*   Updated: 2024/12/10 14:18:05 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"MethodUtils.hpp"

std::string get_file_content(const std::string& path) 
{
    std::ifstream file(path.c_str(), std::ios::in | std::ios::binary);
    if (!file) 
        return "";
    return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

std::string make_true_path(const std::string &uri, const std::string &root_path)
{
    std::string tmp;

    if (root_path.empty())
        return (uri);
    else
        tmp = root_path + uri;
    return (tmp);
}

std::string get_error_page(int status, const ChildServer *serv, std::string default_message)
{
    std::string body;

    // errorPagesの検索
    std::map<int, std::string>::const_iterator it = serv->get_error_page().find(status);
    
    std::cout << status << std::endl;
    // statusに対応するエラーページが見つかった場合
    if (it != serv->get_error_page().end()) 
        body = get_file_content(it->second);
    else
        body = default_message;
    return body;
}

std::string get_file_extension(std::string& file_path)
{
    size_t dot_pos = file_path.find_last_of(".");
    if (dot_pos != std::string::npos) {
        return file_path.substr(dot_pos + 1);  // 拡張子部分を返す
    }
    return "";  // 拡張子がない場合
}

void HTTPResponse::setHeadersContentType(std::string& true_path)
{
    // 拡張子を取得
    std::string extension = get_file_extension(true_path);
    // 拡張子に基づいてContent-Typeを設定
    if (extension == "html") 
        _headers["Content-Type"] = "text/html";
    else if (extension == "jpg" || extension == "jpeg")
        _headers["Content-Type"] = "image/jpeg";
    else if (extension == "png")
        _headers["Content-Type"] = "image/png";
    else if (extension == "txt") 
        _headers["Content-Type"] = "text/plain";
    else if (extension == "json")
        _headers["Content-Type"] = "application/json";
    else if (extension == "pdf") 
        _headers["Content-Type"] = "application/pdf";
    else if (extension == "css") 
        _headers["Content-Type"] = "text/css";
    else if (extension == "js") 
        _headers["Content-Type"] = "application/javascript";
    else if (extension == "xml")
        _headers["Content-Type"] = "application/xml";
    else if (extension == "gif") 
        _headers["Content-Type"] = "image/gif";
    else if (extension == "svg") 
        _headers["Content-Type"] = "image/svg+xml";
    else if (extension == "")
        _headers["Content-Type"] = "application/octet-stream";
    else 
        _headers["Content-Type"] = "application/octet-stream";
}

bool is_dir(const std::string &path)
{
	struct stat pathStat;
	if (stat(path.c_str(), &pathStat) == 0)
		return S_ISDIR(pathStat.st_mode);
	else
		return (false);
}

std::string get_redirect_body(const std::string uri)
{
    std::string html = "<html><body><p>You are being redirected to <a href=\"" + uri + "\">this page</a>.</p></body></html>";
    return (html);
}

// std::vector<Location>::const_iterator find_location(const ChildServer *Server, const std::string& path)
// {
//     const std::vector<Location>& loc = Server->getLocations();
    
//     for (std::vector<Location>::const_iterator it = loc.begin(); it != loc.end(); ++it)
//     {
//         if (path.compare(0, it->getPath().size(), it->getPath()) == 0) 
//             return it;
//     }
//     return Server->getLocations().end();
// }
std::vector<Location>::const_iterator find_location(const ChildServer *Server, const std::string& path)
{
    const std::vector<Location>& loc = Server->getLocations();
    std::vector<Location>::const_iterator best_match = loc.end();
    size_t best_match_length = 0;

    for (std::vector<Location>::const_iterator it = loc.begin(); it != loc.end(); ++it)
    {
        const std::string& loc_path = it->getPath();
        // pathがloc_pathで先頭一致するか確認
        if (path.compare(0, loc_path.size(), loc_path) == 0) 
        {
            // 現在のloc_pathがこれまでの中で最も長い一致であれば更新
            if (loc_path.size() > best_match_length) {
                best_match_length = loc_path.size();
                best_match = it;
            }
        }
    }

    return best_match; // 最長一致がなければend()が返る
}

bool is_max_over(const std::string body, size_t max)
{
    std::cout << "body:" <<body.length() << std::endl;
    std::cout << "max:" << max << std::endl;
    if (body.length() > max)
        return (true);
    else
        return (false);
}