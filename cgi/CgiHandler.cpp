#include "CgiHandler.hpp"

CgiHandler::CgiHandler(HTTPRequest &request) : request_(request)
{
	env_vars_["REQUEST_METHOD"] = request_.getMethod();
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
		char *argv[] = {const_cast<char *>(script_path.c_str()), NULL};
		execve(script_path.c_str(), argv, &(envp[0]));

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

		return ParseCGIresponse(cgi_response); // responseの作成
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
	std::string body = raw_response.substr(header_end + 2);

	std::map<std::string, std::string> headers;
	std::istringstream header_stream(header_section);
	std::string line;
	while (std::getline(header_stream, line) && !line.empty())
	{
		size_t delimiter = line.find(": ");
		if (delimiter != std::string::npos)
		{
			std::string key = line.substr(0, delimiter);
			std::string value = line.substr(delimiter + 2);
			headers[key] = value;
		}
	}

	std::string status_line = "HTTP/1.1 200 OK\r\n";
	if (headers.find("Status") != headers.end())
	{
		status_line = "HTTP/1.1 " + headers["Status"] + "\r\n";
		headers.erase("Status");
	}
	else if (headers.find("Location") != headers.end())
	{
		status_line = "HTTP/1.1 302 Found\r\n";
	}

	std::ostringstream response_stream;
	response_stream << status_line;
	for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); ++it)
	{
		response_stream << it->first << ": " << it->second << "\r\n";
	}
	response_stream << "\r\n";
	response_stream << body;

	return response_stream.str();
}
