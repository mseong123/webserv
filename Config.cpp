#include "Config.hpp"
#include "CustomException.hpp"

Config::Config(std::string path) : _path(path), _file_content("") {
}

Config::~Config() {
}

void Config::read_file(std::fstream & fs) {
	std::string buffer;
	while(std::getline(fs, buffer, '\n')) {
		this->_file_content += buffer;
	}
}

void Config::parse_whitespace(size_t *pos) {
	size_t temp_pos = this->_file_content.find_first_not_of(" \t", *pos);
	if (temp_pos != std::string::npos)
		*pos = temp_pos;
}

void Config::parse_server_block(size_t *pos) {
	size_t temp_pos;
	std::string key[] = {"server", "listen", "server_names", "client_max_body_size", "error"}
	parse_whitespace(pos);
	temp_pos = this->_file_content.find("server", *pos);
	if (temp_pos == std::string::npos) {
		throw CustomException("CONFIG_FILE_ERROR: parsing error at position: " + std::to_string(*pos) + ".." + std::substr(*pos, ))
	}
	std::cout<<*pos<<std::endl;
	
}

void Config::parse_file() {
	std::fstream fs(this->_path, std::fstream::in);
	size_t pos = 0;
	size_t server_block = 0;
	size_t location_block = 0;
	if (!fs.is_open())
		throw CustomException("CONFIG_FILE_ERROR:Invalid config filename or file is missing");
	else
	{
		this->read_file(fs);
		std::cout<<this->_file_content<< std::endl;
		this->parse_server_block(&pos);
		

	}
}