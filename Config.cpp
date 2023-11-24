#include "webserv.hpp"

Config::Config(std::string path) : _path(path), _file_content("") {
}

Config::~Config() {
}

void Config::read_file(std::fstream & fs) {
	std::string buffer;
	while(std::getline(fs, buffer, '\n')) {
		this->_file_content += ' ';
		this->_file_content += buffer;
	}
}

void Config::parse_whitespace(size_t *pos, std::string content) {
	while (*pos != content.length()) 
	{
		if (content[*pos] == '\t' || content[*pos] == ' ')
			*pos += 1;
		else
			break;
	}
}

void Config::parse_file(std::vector<Server> & _servers) {
	std::fstream fs(this->_path, std::fstream::in);
	size_t pos = 0;
	size_t temp_pos = 0;
	if (!fs.is_open())
		throw CustomException("CONFIG_FILE_ERROR:Invalid config filename or file is missing");
	else
	{
		ConfigServer config_server;
		this->read_file(fs);
		while (pos != this->_file_content.length())
		{
			parse_whitespace(&pos, this->_file_content);
			if (pos != this->_file_content.length())
			{
				if (this->_file_content.substr(pos, 6) != "server")
					throw CustomException("CONFIG_FILE_ERROR: server keyword error: " + this->_file_content.substr(pos, 6));
				else {
					pos += 6;
					config_server.parse_server_block(&pos, _servers, this->_file_content);
				}
			}
		}
		if (_servers.empty())
			throw CustomException("CONFIG_FILE_ERROR: need a server block");
	}
}