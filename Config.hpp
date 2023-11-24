#ifndef CONFIG_HPP
#define CONFIG_HPP
#include <fstream>

class Server;
class Location;
class ConfigServer;

class Config {
	public:
		Config(std::string path);
		~Config();
		void parse_file(std::vector<Server> & _servers);
		void read_file(std::fstream & fs);
		void parse_whitespace(size_t *pos, std::string content);
	private:
		std::string _path;
		std::string _file_content;
};

#endif