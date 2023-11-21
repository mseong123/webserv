#include "Config.hpp"
#include "CustomException.hpp"

Config::Config(std::string path) : _path(path), _file_content("") {
}

Config::~Config() {
}

void Config::read_file(std::fstream & fs) {
	while(std::getline(fs, this->_file_content, '\n')) {
		this->_file_content += this->_file_content;
	}
}

void Config::parse_file() {
	std::fstream fs(this->_path, std::fstream::in);
	if (!fs.is_open())
		throw CustomException("CONFIG ERROR:Invalid config filename or file is missing");
	else
	{
		this->read_file(fs);
		std::cout<<this->_file_content<< std::endl;

	}
}