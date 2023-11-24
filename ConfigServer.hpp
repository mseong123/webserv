#ifndef CONFIGSERVER_HPP
#define CONFIGSERVER_HPP

class ConfigLocation;

class ConfigServer {
	public:
		ConfigServer();
		~ConfigServer();
		void parse_whitespace(size_t *pos, std::string content);
		void check_semicolon(size_t *pos, std::string keyword, std::string file_content);
		void parse_address(Server & server);
		void parse_listen(size_t *pos, Server & server, std::string file_content);
		void parse_server_name(size_t *pos, Server & server, std::string file_content);
		void parse_client_max_body_size(size_t *pos, Server & server, std::string file_content);
		void parse_error_pages(size_t *pos, Server & server, std::string file_content);
		void parse_server_block(size_t *pos, std::vector<Server> & _server, std::string file_content);
};

#endif