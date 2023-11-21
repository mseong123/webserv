#ifndef CONFIG_HPP
#define CONFIG_HPP
#include <fstream>

class Server;

class Config {
	public:
		Config(std::string path);
		~Config();
		void parse_file(std::vector<Server> & _servers);
		void read_file(std::fstream & fs);
		void parse_server_block(size_t *pos, std::vector<Server> & _servers);
		void parse_location_block(size_t *pos);
		void parse_listen(size_t *pos, Server & server);
		void parse_whitespace(size_t *pos);
		void check_semicolon(size_t *pos, std::string keyword);
		
	private:
		std::string _path;
		std::string _file_content;
};

#endif