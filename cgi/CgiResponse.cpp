#include "CgiResponse.hpp"

typedef std::map<std::string, std::string> HeaderMap;

HeaderMap ParseHeaders(const std::string &header_string)
{
	HeaderMap header_map;
	std::istringstream header_stream(header_string);
	std::string line;
	size_t colon_pos;

	while (std::getline(header_stream, line))
	{
		if (line.empty() || line == "\r")
		{
			break;
		}
		colon_pos = line.find(':');
		if (colon_pos != std::string::npos)
		{
			std::string key = line.substr(0, colon_pos);
			std::string value = line.substr(colon_pos + 1);
			while (!value.empty() && (value[0] == ' ' || value[0] == '\t')) // trim処理
			{
				value = value.substr(1);
			}
			if (!key.empty() && !value.empty())
			{
				header_map[key] = value;
			}
		}
		colon_pos = std::string::npos;
	}
	return header_map;
}

std::string DetermineResponseType(const HeaderMap &headers, const std::string &body)
{
	std::string response_type = "document-response";

	if (headers.find("Location") != headers.end())
	{
		if (headers.at("Location").find("http://") != std::string::npos)
		{
			response_type = "client-redir-response";
		}
		else
		{
			response_type = "local-redir-response";
		}
	}

	if (response_type == "client-redir-response" && !body.empty())
	{
		response_type = "client-redirdoc-response";
	}

	return response_type;
}

std::string ParseCGIResponse(const std::string &response)
{
	std::string result = "";

	size_t header_end = response.find("\r\n\r\n");
	if (header_end == std::string::npos)
	{
		result = "Invalid CGI response: no headers found.\n";
		return result;
	}
	std::string header_part = response.substr(0, header_end);
	std::string body_part = response.substr(header_end + 4);

	HeaderMap headers = ParseHeaders(header_part);

	result = "HTTP/1.1 200 OK\r\n";
	if (headers.find("Status") != headers.end())
	{
		result = "HTTP/1.1 " + headers["Status"] + "\r\n";
	}

	std::string response_type = DetermineResponseType(headers, body_part);

	std::cout << "response_type: " << response_type << std::endl;
	result = "HTTP/1.1 ";
	if (response_type == "document-response")
	{
		result += "200 OK\r\n";
		result += "Content-Type: " + headers["Content-Type"] + "\r\n";
		result += "\r\n" + body_part;
	}
	else if (response_type == "local-redir-response")
	{
		std::string location = headers["Location"];
		if (location.empty())
		{
			result += "400 Bad Request\r\n\r\nLocation header not found.\n";
		}
		else
		{
			result += "302 Found\r\n";
			result += "Location: " + location + "\r\n\r\n";
		}
	}
	else if (response_type == "client-redir-response")
	{
		result += "Location: " + headers["Location"] + "\n";
	}
	else if (response_type == "client-redirdoc-response")
	{
		result += "Location: " + headers["Location"] + "\n";
		result += "Body: " + body_part + "\n";

	}
	else
	{
		result = "500 Internal Server Error\r\n\r\nUnknown response type.\n";
	}
	return result;
}
