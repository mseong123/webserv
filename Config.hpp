#ifndef CONFIG_HPP
#define CONFIG_HPP
#include <fstream>

class Config {
	public:
		Config(std::string path);
		~Config();
		void parse_file();
		void read_file(std::fstream & fs);
		
	private:
		std::string _path;
		std::string _file_content;
};

#endif