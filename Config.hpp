#ifndef CONFIG_HPP
#define CONFIG_HPP
#include <fstream>

class Config {
	public:
		Config(std::string path);
		~Config();
		void parse_file();
		void read_file(std::fstream & fs);
		void parse_server_block(size_t *pos);
		void parse_whitespace(size_t *pos);
		bool check_key(std::string key);
		
	private:
		std::string _path;
		std::string _file_content;
		std::string _server_keys[4];
};

#endif