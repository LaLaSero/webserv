#include "HTTPResponse.hpp"

#include <string>
#include <vector>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <dirent.h>
#include <sys/stat.h>
#include <cstdio>

bool HTTPResponse::indexFileExist(HTTPRequest& request)
{
	(void)request;
	// indexファイルが存在するかどうか
	return false;
}

bool HTTPResponse::isAutoIndexEnabled(HTTPRequest& request)
{
	(void)request;
	// serverConfigでautoIndexが有効になっているかどうか
	return false;
}

// void HTTPResponse::makeBodyAutoIndex(std::string files, std::string uri, std::stringstream& ss)
// {
// 	// Location location;
// 	// 自動インデックスの処理
// }

bool HTTPResponse::isDirectoryRequest(HTTPRequest& request)
{
	// (void)request;
	// リクエストされたURIがディレクトリかどうか
    (void)request;
	return false;
}

bool HTTPResponse::hasTrailingSlash(const std::string& uri) const
{
	return !uri.empty() && uri[uri.size() - 1] == '/';
}

void HTTPResponse::redirectToTrailingSlash(const HTTPRequest& request)
{
	std::string uri = request.getUri();
	if (uri[uri.size() - 1] != '/') {
		uri += "/";
	}
	_statusCode = STATUS_301;
	_headers["Location"] = uri;
	_body = "<html><head><title>301 Moved Permanently</title></head>"
			"<body><h1>301 Moved Permanently</h1>"
			"<p>Redirecting to " + uri + "</p></body></html>";
	makeMessage();
	// std::cout << message << std::endl;
}

std::string HTTPResponse::mapUriToPath(std::string uri)
{
	(void)uri;
	std::string path;
	// URIをファイルパスに変換
	return path;
}

std::vector<FileInfo> HTTPResponse::readDirectoryContents(std::string path)
{
    std::vector<FileInfo> fileList;

    // ディレクトリをオープン
    DIR *dir = opendir(path.c_str());
    if (dir == NULL) 
    {
        return fileList;
    }

    struct dirent *entry;
    struct stat fileStat;

    // ディレクトリの内容を1つずつ読み込む
    while ((entry = readdir(dir)) != NULL) {
        // "." と ".." は無視
        if (entry->d_name[0] == '.') {
            continue;
        }

        // ファイルのパスを作成
        char filePath[1024];
        sprintf(filePath, "%s/%s", path.c_str(), entry->d_name);

        // stat を使ってファイルの情報を取得
        if (stat(filePath, &fileStat) == 0) {
            FileInfo fileInfo;
            fileInfo.name = entry->d_name;          // ファイル名
            fileInfo.size = fileStat.st_size;       // ファイルのサイズ
            fileInfo.mtime = fileStat.st_mtime;     // 最終更新日時

            // ファイル情報をリストに追加
            fileList.push_back(fileInfo);
        }
    }

    // ディレクトリをクローズ
    closedir(dir);
    return fileList;
}


std::string escapeHTML(const std::string& str) {
    std::string result;
    for (size_t i = 0; i < str.length(); ++i) {
        switch (str[i]) {
            case '&': result += "&amp;"; break;
            case '<': result += "&lt;"; break;
            case '>': result += "&gt;"; break;
            case '"': result += "&quot;"; break;
            case '\'': result += "&apos;"; break;
            default: result += str[i]; break;
        }
    }
    return result;
}

// 日付を "28-Sep-1970 12:00" の形式でフォーマットする関数
std::string formatTime(time_t mtime) {
    struct tm timeinfo;
    localtime_r(&mtime, &timeinfo);  // thread-safeなlocaltime関数

    char buffer[80];
    static const char* months[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
    sprintf(buffer, "%02d-%s-%04d %02d:%02d",
            timeinfo.tm_mday, months[timeinfo.tm_mon], (1900 + timeinfo.tm_year),
            timeinfo.tm_hour, timeinfo.tm_min);
    return std::string(buffer);
}

// サイズを人間が読みやすい形式でフォーマットする関数 (nginxスタイル)
std::string formatSize(off_t size) {
    char sizeStr[20];
    if (size == 0) {
        return "                  -"; // ディレクトリの場合
    }

    if (size >= 1024 * 1024 * 1024) {
        sprintf(sizeStr, "%6iG", (int)(size / (1024 * 1024 * 1024)));
    } else if (size >= 1024 * 1024) {
        sprintf(sizeStr, "%6iM", (int)(size / (1024 * 1024)));
    } else if (size >= 1024) {
        sprintf(sizeStr, "%6iK", (int)(size / 1024));
    } else {
        sprintf(sizeStr, " %6i", (int)size);
    }
    return std::string(sizeStr);
}

std::string computeParentUri(const std::string& uri)
{
    if (uri.empty() || uri == "/") return "/";
    std::string temp = uri;
    if (temp[temp.size() - 1] == '/')
        temp.erase(temp.size() - 1);
    // temp.pop_back();
    size_t pos = temp.find_last_of('/');
    if (pos == std::string::npos) return "/";
    if (pos == 0) return "/";
    return temp.substr(0, pos) + "/";
}

std::string HTTPResponse::generateAutoIndexHTML(const std::vector<FileInfo>& fileList, const std::string& uri) 
{
    std::string htmlContent;
    
    std::string parentUri = computeParentUri(uri);

    // HTMLのヘッダー部分
    htmlContent += "<html><head><title>Index of " + uri + "</title></head><body>";
    htmlContent += "<h1>Index of " + uri + "</h1>";
    htmlContent += "<hr><pre><a href=\"" + parentUri + "\">../</a>\n";
    // ファイルリストをリスト形式で追加
    for (size_t i = 0; i < fileList.size(); ++i) {
        const FileInfo& file = fileList[i];
        
        // ファイル名のエスケープ（例: <, >, & など）
        std::string escapedName = escapeHTML(file.name);
        
        // 日付の形式をnginxと同じく "28-Sep-1970 12:00" に変換
        std::string lastModified = formatTime(file.mtime);

        // サイズを適切な形式で表示
        std::string sizeStr = formatSize(file.size);

        // ディレクトリの場合は末尾に '/' を付加
        std::string fileLink = (file.size == 0) ? escapedName + "/" : escapedName;
        std::string linkUri = uri;
        if (!linkUri.empty() && linkUri[linkUri.size() - 1] != '/') {
            linkUri += "/";
        }

        // ファイル名、サイズ、最終更新日時を表示
        htmlContent += "<a href=\"" + linkUri + escapedName + "\">" + fileLink + "</a>";
        
        // サイズの表示 (ディレクトリの場合は "-" を表示)
        htmlContent += " " + sizeStr;
        
        // 最終更新日時を表示
        htmlContent += " " + lastModified + "\n";
    }

    // HTMLのフッター部分
    htmlContent += "</pre><hr></body></html>";

    return htmlContent;
}


void HTTPResponse::serveAutoIndex(std::string htmlContent, HTTPRequest& request)
{
	(void)htmlContent;
	(void)request;
	// 自動インデックスのレスポンスを返す
}

void HTTPResponse::handleAutoIndex(HTTPRequest& request)
{
	std::string uri = request.getUri();
	std::string path = mapUriToPath(uri);
	if(!isDirectoryRequest(request))
	{
		// normalリクエストに飛ばす
		handleNormalRequest(request);
		return ;
	}
	if (!hasTrailingSlash(uri))
	{
		redirectToTrailingSlash(request);
		return ;
	}
	if (indexFileExist(request))
	{
		handleNormalRequest(request);
		return ;
	}
	 if (!isAutoIndexEnabled(request))
	{
		_statusCode = STATUS_403;
		_body = "<html><head><title>403 Forbidden</title></head>"
				"<body><h1>403 Forbidden</h1><p>Directory listing is forbidden.</p></body></html>";
		makeMessage();
		// std::cout << message << std::endl;
		return ;
	}

	// 自動インデックスの処理
	std::vector<FileInfo> fileList = readDirectoryContents(path);
	std::string htmlContent = generateAutoIndexHTML(fileList, uri);
	serveAutoIndex(htmlContent, request);

}