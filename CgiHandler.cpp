#include "webserv.hpp"

CgiHandler::CgiHandler() {
}

CgiHandler::~CgiHandler() {
}

std::string	CgiHandler::parse_cgi_path(Request & request, Location & location) {
	std::string cgi_path = ".";

	cgi_path += location.get_cgi_pass();
	cgi_path += request.get_route();
	return cgi_path;
}

std::string	CgiHandler::parse_cgi_output(int stdoutput_pipe) {
	char buffer;
	std::string output;
	int rec_byte;

	while ((rec_byte = read(stdoutput_pipe, &buffer, 1)) != 0)
		output += buffer;
	return output;
}

char **CgiHandler::set_env(Request & request) {
	std::map<std::string, std::string>::iterator it = request.get_header_fields().begin();
	std::map<std::string, std::string>::iterator ite = request.get_header_fields().end();
	char **envp = (char **)malloc((request.get_header_fields().size() + 2) * sizeof(char *));
	int i = 0;

	envp[i] = strdup((std::string("REQUEST_METHOD=") + request.get_method()).c_str());
	i++;
	for (; it != ite; it++) {
		std::string field = it->first;
		size_t pos = field.find_first_of("-");
		std::transform(field.begin(), field.end(), field.begin(), ::toupper);
		if (pos != std::string::npos)
			field.replace(pos, 1, "_");
		envp[i] = strdup((char *)std::string(field + "=" + it->second).c_str());
		i++;
	}
	envp[i] = NULL;
	return envp;
}

void CgiHandler::handle_cgi(Request & request, Response & response, Server & virtual_server, Location & location) {
	std::string cgi_path = parse_cgi_path(request, location);
	char *argv[2] = {(char *)cgi_path.c_str(), NULL};
	int pid;
	int pipefd_stdoutput[2];
	int pipefd_stdinput[2];
	pipe(pipefd_stdinput);
	write(pipefd_stdinput[1], request.get_message_body().c_str(), request.get_message_body().length());
	close(pipefd_stdinput[1]);
	std::cout << request.get_message_body().length() <<std::endl;
	if (pipe(pipefd_stdoutput) == -1)
		response.parse_error_pages("500", "Internal Server Error", virtual_server);
	else {
		
		pid = fork();
		if (pid == -1)
			response.parse_error_pages("500", "Internal Server Error", virtual_server);
		else if (pid == 0) {
			dup2(pipefd_stdinput[0], STDIN_FILENO);
			dup2(pipefd_stdoutput[1], STDOUT_FILENO);
			close(pipefd_stdoutput[0]);
			close(pipefd_stdoutput[1]);
			close(pipefd_stdinput[1]);
			close(pipefd_stdinput[0]);
			char **envp = this->set_env(request);
			// std::cout<<getenv("CONTENT-LENGTH") << std::endl;
			if (execve(cgi_path.c_str(), argv, envp) == -1)
			{
				std::cout <<"here" << cgi_path.c_str()<<std::endl;
				perror("execve");
				response.parse_error_pages("500", "Internal Server Error", virtual_server);
				write(STDOUT_FILENO, response.get_data().c_str(), response.get_data().length());
				exit(EXIT_FAILURE);
			}
		}
		else {
				close(pipefd_stdoutput[1]);
				wait(NULL);
				std::string cgi_body;
				std::string output;

				cgi_body = this->parse_cgi_output(pipefd_stdoutput[0]);
				output += std::string(HTTP_PROTOCOL) + " 200 OK\r\n";
				output += "Content-Type: text/html\r\n";
				output += "Content-Length: " + std::to_string(cgi_body.length()) + "\r\n";
				output += "\r\n";
				output += cgi_body;
				response.set_data(output);
				close(pipefd_stdoutput[0]);
			}
		}
}
