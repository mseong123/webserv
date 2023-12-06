#ifndef CGIHANDLER_HPP
#define CGIHANDLER_HPP

class CgiHandler {
	public:
		CgiHandler(void);
		~CgiHandler();
		std::string	parse_cgi_path(Location & location);
		std::string	parse_cgi_output(int stdoutput_pipe, int stderror_pipe);
		char **set_env(Request & request);
		std::string parse_ext(Request & request);
		void handle_cgi(Request & request, Response & response, Server & virtual_server, Location & location);
};

#endif