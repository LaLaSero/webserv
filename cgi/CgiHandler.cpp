#include "CgiHandler.hpp"

CgiHandler::CgiHandler(HTTPRequest &request) : request_(request)
{
	env_vars_["REQUEST_METHOD"] = request_.getMethod();
	env_vars_["REQUEST_METHOD"] = "GET"; //for test
	env_vars_["QUERY_STRING"] = request_.getQuery();
	env_vars_["SCRIPT_NAME"] = request_.getUri(); 
	env_vars_["CONTENT_TYPE"] = "test";
	env_vars_["CONTENT_LENGTH"] = request_.getBody().size();
	env_vars_["PATH_INFO"] = request_.getUri();
	env_vars_["REQUEST_URI"] = request_.getUri() + request_.getQuery();
	env_vars_["SERVER_PROTOCOL"] = request_.getVersion();
	env_vars_["SERVER_SOFTWARE"] = "42_WebServ";
	env_vars_["SERVER_NAME"] = request_.getHost();
	env_vars_["GATEWAY_INTERFACE"] = "CGI/1.1";
	env_vars_["REMOTE_ADDR"] = "IP of the client";
	env_vars_["SERVER_PORT"] = request_.getPort();
	request_body_ = request_.getBody();
}

void CgiHandler::setRequestBody(const std::string &body)
{
	request_body_ = body;
}

std::string CgiHandler::ExecuteCGI()
{
	int input_pipe[2], output_pipe[2];
	if (pipe(input_pipe) == -1 || pipe(output_pipe) == -1)
	{
		perror("pipe");
		return "500 Internal Server Error";
	}

	pid_t pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return "500 Internal Server Error";
	}
	else if (pid == 0) // child process
	{
		close(input_pipe[1]);
		close(output_pipe[0]);

		dup2(input_pipe[0], STDIN_FILENO);
		dup2(output_pipe[1], STDOUT_FILENO);

		close(input_pipe[0]);
		close(output_pipe[1]);

		// 環境変数を設定
		std::vector<char *> envp;
		for (std::map<std::string, std::string>::const_iterator it = env_vars_.begin(); it != env_vars_.end(); ++it)
		{
			std::string env_pair = it->first + "=" + it->second;
			envp.push_back(strdup(env_pair.c_str()));
		}
		envp.push_back(NULL);

		std::string script_path = env_vars_["SCRIPT_NAME"];
		std::string python_path = "python3";
		char *argv[] = {const_cast<char *>(python_path.c_str()), 
						const_cast<char *>(script_path.c_str()),
						NULL};
		execve(const_cast<char *>(python_path.c_str()), argv, &(envp[0]));

		perror("execve");
		for (size_t i = 0; i < envp.size(); ++i)
		{
			free(envp[i]);
		}
		std::exit(1);
	}
	else // parent process
	{
		close(input_pipe[0]);
		close(output_pipe[1]);

		if (!request_body_.empty())
		{
			write(input_pipe[1], request_body_.c_str(), request_body_.size());
		}
		close(input_pipe[1]);

		// CGIからのレスポンスを読み取る
		std::string cgi_response;
		char buffer[1024];
		ssize_t read_bytes;
		while ((read_bytes = read(output_pipe[0], buffer, sizeof(buffer))) > 0)
		{
			cgi_response.append(buffer, read_bytes);
		}
		close(output_pipe[0]);

		int status;
		
		waitpid(pid, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
		{
			return ParseCGIresponse(cgi_response); // responseの作成
		}
		else
		{
			return "500 Internal Server Error";
		}
	}
}



std::string CgiHandler::ParseCGIresponse(const std::string &raw_response)
{

	size_t header_end = raw_response.find("\r\n\r\n");
	if (header_end == std::string::npos)
	{
		header_end = raw_response.find("\n\n");
		if (header_end == std::string::npos)
		{
			// ヘッダーが存在しない場合
			return "500 Internal Server Error";
		}
	}

	std::string header_section = raw_response.substr(0, header_end);
	std::string body = raw_response.substr(header_end + 4); // Skip the header ending

	std::map<std::string, std::string> headers_table;
	std::istringstream header_stream(header_section);
	std::string line;
	while (std::getline(header_stream, line) && !line.empty())
	{
		if (line[line.size() - 1] == '\r')
			line.erase(line.size() - 1); // Remove carriage return
		size_t delimiter = line.find(": ");
		if (delimiter != std::string::npos)
		{
			std::string key = line.substr(0, delimiter);
			std::string value = line.substr(delimiter + 2);
			headers_table[key] = value;
		}
	}

	std::string status_line = "HTTP/1.1 200 OK\r\n"; // default statusの設定
	if (headers_table.find("Status") != headers_table.end()) // Statusの確認
	{
		status_line = "HTTP/1.1 " + headers_table["Status"] + "\r\n";
		headers_table.erase("Status");
	}

	if (headers_table.find("Location") != headers_table.end()) // Redirectの確認
	{
		std::string location = headers_table["Location"];
		if (!location.empty() && location[0] == '/') // Local redirect
		{
			std::cout << "Local redirect: " << location << std::endl;
			// Handle local redirect (internal redirect)
			std::string internal_redirect_uri_ = location;
			return "";
		}
		else
		{
			// External redirect
			status_line = "HTTP/1.1 302 Found\r\n";
		}
	}

	std::ostringstream response_stream;
	response_stream << status_line;
	for (std::map<std::string, std::string>::iterator it = headers_table.begin(); it != headers_table.end(); ++it)
	{
		response_stream << it->first << ": " << it->second << "\r\n";
	}
	response_stream << "\r\n";

	return response_stream.str();
}
