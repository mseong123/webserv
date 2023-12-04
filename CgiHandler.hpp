#ifndef CGIHANDLER_HPP
#define CGIHANDLER_HPP

class CgiHandler {
	public:
		CgiHandler(void);
		~CgiHandler();
		std::string	parse_cgi_path(Request & request, Location & location);
		std::string	parse_cgi_output(int stdoutput_pipe);
		char **set_env(Request & request);
		void handle_cgi(Request & request, Response & response, Server & virtual_server, Location & location);
};

#endif