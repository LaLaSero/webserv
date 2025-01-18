#include "CgiHandler.hpp"

CgiHandler::CgiHandler(HTTPRequest &request) : request_(request)
{
	env_vars_["REQUEST_METHOD"] = request_.getMethod();
	env_vars_["QUERY_STRING"] = request_.getQuery();

	std::string tmp = request_.getUri();
	size_t pos = tmp.find("?");
	std::string script_name;
	if (pos != std::string::npos)
	{
		script_name = tmp.substr(0, pos);
		env_vars_["SCRIPT_NAME"] = script_name;
	}
	env_vars_["CONTENT_TYPE"] = "test";
	env_vars_["CONTENT_LENGTH"] = request_.getBody().size();
	env_vars_["PATH_INFO"] = request_.getUri();
	env_vars_["REQUEST_URI"] = request_.getUri();
	env_vars_["SERVER_PROTOCOL"] = request_.getVersion();
	env_vars_["SERVER_SOFTWARE"] = "webserv";
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

// CGIのディレクトリパスがconfigから設定されているかを確認
bool CgiHandler::is_valid_root_and_executer_cgi()
{
	if (request_.getLocation().getRootDirectory().empty())
	{
		std::cerr << "Root directory is not set in config file" << std::endl;
		return false;
	}
	return true;
}

std::vector<char *> CgiHandler::makeEnvp() const
{
	std::vector<char *> envp;
	for (std::map<std::string, std::string>::const_iterator it = env_vars_.begin(); it != env_vars_.end(); ++it)
	{
		std::string env_pair = it->first + "=" + it->second;
		char *env = strdup(env_pair.c_str());
		if (env == NULL)
		{
			perror("strdup");
			std::exit(1);
		}
		envp.push_back(env);
	}
	envp.push_back(NULL);
	return envp;
}

std::string getScriptFileName(const std::string &uri)
{
	std::string script_file_name;
	size_t pos = uri.find_last_of('/');
	if (pos != std::string::npos)
	{
		script_file_name = uri.substr(pos + 1);
	}
	pos = script_file_name.find("?");
	if (pos != std::string::npos)
	{
		script_file_name = script_file_name.substr(0, pos);
	}
	return script_file_name;
}

std::string getCgiDirectory(Location &location, HTTPRequest &request)
{
	std::string cgi_dir_head = location.getRootDirectory();
	std::string cgi_dir_tail = request.getLocation().getPath() + "cgi-bin/";
	cgi_dir_head.erase(cgi_dir_head.length() - 1);
	std::string cgi_dir = cgi_dir_head + cgi_dir_tail;

	return cgi_dir;
}

bool isAccessForbidden(const std::string &script_file_name)
{
	if (access(script_file_name.c_str(), F_OK) == -1)
	{
		perror("access");
		std::cerr << "404 Not Found" << std::endl;
		return true;
	}
	return false;
}

void ExecuteChildCGI(int *output_pipe, HTTPRequest request, Location location)
{
	CgiHandler cgi_handler(request);

	if (!cgi_handler.is_valid_root_and_executer_cgi())
	{
		std::cerr << "500 Internal Server Error" << std::endl;
		std::exit(1);
	}

	dup2(output_pipe[1], STDOUT_FILENO);
	close(output_pipe[0]);
	close(output_pipe[1]);

	std::map<std::string, std::string> env_vars = cgi_handler.getEnvVars();
	std::vector<char *> envp = cgi_handler.makeEnvp();

	std::string script_file_name = getScriptFileName(request.getUri());
	std::string python_path = "/usr/bin/python3";

	std::string cgi_dir = getCgiDirectory(location, request);
	if (chdir(cgi_dir.c_str()) == -1)
	{
		perror("chdir");
		std::exit(1);
	}

	if (isAccessForbidden(script_file_name))
	{
		std::cerr << "403 Forbidden" << std::endl;
		std::exit(1);
	}

	char *argv[] = {const_cast<char *>(python_path.c_str()),
					const_cast<char *>(script_file_name.c_str()),
					NULL};
	execve(const_cast<char *>(python_path.c_str()), argv, &(envp[0]));

	perror("execve");
	for (size_t i = 0; i < envp.size(); ++i)
	{
		free(envp[i]);
	}
	std::exit(1);
}

std::map<std::string, std::string> CgiHandler::getEnvVars() const
{
	return env_vars_;
}