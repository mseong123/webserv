#include "webserv.hpp"

Config::Config(std::string path) : _path(path), _file_content(""), _server_keys() {
	this->_server_keys[0] = "listen";
	this->_server_keys[1] = "server_names";
	this->_server_keys[2] = "client_max_body_size";
	this->_server_keys[3] = "error_pages";
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

bool Config::check_key(std::string key) {
	for (size_t i = 0; i < sizeof(this->_server_keys) / sizeof(this->_server_keys[0]); i++)
	{
		if (key == this->_server_keys[i])
			return true;
	}
	return false;
}

void Config::parse_server_block(size_t *pos) {
	size_t temp_pos;
	while (*pos != this->_file_content.length())
	{
		parse_whitespace(pos);
		temp_pos = this->_file_content.find("server", *pos);

	}
	temp_pos = this->_file_content.find("server", *pos);
	if (temp_pos == std::string::npos)
		throw CustomException("CONFIG_FILE_ERROR: parsing error at position: " + std::to_string(*pos) + " keyword error:" + this->_file_content.substr(*pos, std::string("server").length()));
	else
		*pos = temp_pos + std::string("server").length();
	parse_whitespace(pos);
	temp_pos = this->_file_content.find("{", *pos);
	if (temp_pos == std::string::npos) {
		throw CustomException("CONFIG_FILE_ERROR: parsing error at position: " + std::to_string(*pos) + " syntax error: missing {");
	}

	
}

void Config::parse_file() {
	std::fstream fs(this->_path, std::fstream::in);
	size_t pos = 0;
	if (!fs.is_open())
		throw CustomException("CONFIG_FILE_ERROR:Invalid config filename or file is missing");
	else
	{
		this->read_file(fs);
		std::cout<<this->_file_content.length()<< std::endl;
		while (pos != this->_file_content.length())
		{
			this->parse_server_block(&pos);
		}
	}
}