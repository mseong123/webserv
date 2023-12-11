#include "webserv.hpp"

CgiHandler::CgiHandler() {
}

CgiHandler::~CgiHandler() {
}

std::string	CgiHandler::parse_cgi_path(Location & location) {
	std::string cgi_path = ".";

	cgi_path += location.get_cgi_pass();
	return cgi_path;
}

std::string	CgiHandler::parse_cgi_output(int pipefd_output, int pipefd_error) {
	char buffer[1024];
	std::string output;
	int rec_byte;

	while ((rec_byte = read(pipefd_output, buffer, 1024)) != 0)
		output.append(buffer, rec_byte);
	while ((rec_byte = read(pipefd_error, buffer, 1024)) != 0)
		output.append(buffer,rec_byte);
	return output;
}

char **CgiHandler::set_env(Request & request) {
	
	std::map<std::string, std::string>::iterator it = request.get_header_fields().begin();
	std::map<std::string, std::string>::iterator ite = request.get_header_fields().end();
	char **envp = (char **)malloc((request.get_header_fields().size() + 3) * sizeof(char *));
	
	int i = 0;
	envp[i] = strdup((std::string("REQUEST_METHOD=") + request.get_method()).c_str());
	i++;
	envp[i] = strdup((std::string("ROUTE=") + request.get_route()).c_str());
	i++;
	for (; it != ite; it++) {
		std::string field = it->first;
		size_t pos = field.find_first_of("-");
		while(pos != std::string::npos) {
			field.replace(pos, 1, "_");
			pos = field.find_first_of("-");
		}
		std::transform(field.begin(), field.end(), field.begin(), ::toupper);
		envp[i] = strdup((char *)std::string(field + "=" + it->second).c_str());
		setenv(field.c_str(),it->second.c_str(), 1);
		i++;
	}
	envp[i] = NULL;
	return envp;
}

void CgiHandler::handle_cgi(Request & request, Response & response, Server & virtual_server, Location & location) {
	std::string cgi_path = parse_cgi_path(location);
	std::string cgi_dir = cgi_path.substr(0, cgi_path.find_first_of("/", 2) + 1);
	char *argv[3] = {(char *)cgi_path.c_str(), (char *)cgi_dir.c_str(), NULL};
	int pid1;
	int pid2;
	int i = 2;
	int pipefd_input[2];
	int pipefd_output[2];
	int pipefd_stderror[2];

	if (pipe(pipefd_input) == -1 || pipe(pipefd_output) == -1 || pipe(pipefd_stderror) == -1)
		response.parse_error_pages("500", "Internal Server Error", virtual_server);
	else {
		while (i > 0) {
			if (i == 2 && (pid1 = fork())==0) {
				dup2(pipefd_input[1], STDOUT_FILENO);
				close(pipefd_input[0]);
				close(pipefd_input[1]);
				close(pipefd_output[1]);
				close(pipefd_output[1]);
				close(pipefd_stderror[0]);
				close(pipefd_stderror[1]);
				write(STDOUT_FILENO, request.get_message_body().c_str(), request.get_message_body().length());
				exit(0);
			}
			else if (i == 1 && (pid2=fork())==0) {
				dup2(pipefd_input[0], STDIN_FILENO);
				close(pipefd_input[0]);
				close(pipefd_input[1]);
				dup2(pipefd_output[1], STDOUT_FILENO);
				close(pipefd_output[1]);
				close(pipefd_output[0]);
				dup2(pipefd_stderror[1], STDERR_FILENO);
				close(pipefd_stderror[0]);
				close(pipefd_stderror[1]);
				char **envp = this->set_env(request);
				if (execve(cgi_path.c_str(), argv, envp) == -1) {
					response.parse_error_pages("500", "Internal Server Error", virtual_server);
					write(STDOUT_FILENO, response.get_data().c_str(), response.get_data().length());
					exit(3);
				}
			}
			i--;
		}
		close(pipefd_input[1]);
		close(pipefd_output[1]);
		close(pipefd_stderror[1]);
		int exit_status;

		std::string cgi_body;
		cgi_body = this->parse_cgi_output(pipefd_output[0], pipefd_stderror[0]);
		waitpid(pid2, &exit_status, 0);
		if (WIFEXITED(exit_status) && WEXITSTATUS(exit_status) == 3) {
			response.set_data(cgi_body);
		}
		else {
			if (request.get_method() == "POST") {
				if (cgi_body.substr(0,8) != "HTTP/1.1") {
					std::string output;
					output += std::string(HTTP_PROTOCOL) + " 200 OK\r\n";
					output += "Content-Type: text/html\r\n";
					output += "Content-Length: " + std::to_string(cgi_body.length()) + "\r\n";
					output += "\r\n";
					output += cgi_body;
					response.set_data(output);
				}
				else
					response.set_data(cgi_body);
			}
			else if (request.get_method() == "GET") {
				if (cgi_body.substr(0,8) != "HTTP/1.1") {
					std::string output;
					output += std::string(HTTP_PROTOCOL) + " 200 OK\r\n";
					output += "Content-Type: text/html\r\n";
					output += "Content-Length: " + std::to_string(cgi_body.length()) + "\r\n";
					output += "\r\n";
					output += cgi_body;
					response.set_data(output);
				}
				else
					response.set_data(cgi_body);
			}
		}
		close(pipefd_input[0]);
		close(pipefd_output[0]);
		close(pipefd_stderror[0]);
	}
}
