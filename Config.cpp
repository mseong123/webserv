#include "webserv.hpp"

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

// void Config::parse_location_block(size_t *pos) {

// }

void Config::check_semicolon(size_t *pos, std::string keyword) {
	this->parse_whitespace(pos);
	if (this->_file_content[*pos] == ';')
		*pos += 1;
	else
		throw CustomException("CONFIG_FILE_ERROR: no semicolon in " + keyword + " directive");
}

void Config::parse_listen(size_t *pos, Server & server) {
	size_t temp_pos = *pos;

	parse_whitespace(pos);
	if (temp_pos == *pos)
		throw CustomException("CONFIG_FILE_ERROR: need whitespace after listen keyword");

	if ((temp_pos = this->_file_content.find(":", *pos)) != std::string::npos) {
		std::string host = this->_file_content.substr(*pos, temp_pos - *pos);
		if (host.find_first_not_of("0123456789.") != std::string::npos)
			throw CustomException("CONFIG_FILE_ERROR: Incorrect host value");
		if (host == "")
			server.set_host("0.0.0.0");
		else
			server.set_host(host);
		*pos = temp_pos + 1;
	}
	
	if (this->_file_content.find_first_of("0123456789", *pos) == std::string::npos) {
		throw CustomException("CONFIG_FILE_ERROR: need a host:name value in listen directive");
	}
	else {
		if (server.get_host() == "")
			server.set_host("0.0.0.0");
		temp_pos = this->_file_content.find_first_not_of("0123456789", *pos);
		server.set_port(this->_file_content.substr(*pos, temp_pos - *pos));
		*pos = temp_pos;
		
		this->check_semicolon(pos, "listen");
	}
}

void Config::parse_server_block(size_t *pos, std::vector<Server> & _servers) {
	bool closing_bracket = false;
	Server server;
	_servers.push_back(server);

	parse_whitespace(pos);
	if (this->_file_content[*pos] != '{') 
		throw CustomException("CONFIG_FILE_ERROR: syntax error: missing { in server block");
	else
		*pos += 1;

	while (*pos != this->_file_content.length()) {
		parse_whitespace(pos);
		if (this->_file_content.find("listen", *pos) != std::string::npos) {
			*pos += 6;
			this->parse_listen(pos, server);
		} 
		else if (this->_file_content.find("}", *pos) != std::string::npos) {
			*pos += 1;
			closing_bracket = true;
			break;
		}
		else {
			throw CustomException("CONFIG_FILE_ERROR: Incorrect directive in server block");
		}
	}
	if (!closing_bracket)
		throw CustomException("CONFIG_FILE_ERROR: syntax error: missing } in server block");
	std::cout << server.get_host() << std::endl;
	std::cout << server.get_port() << std::endl;

}

void Config::parse_file(std::vector<Server> & _servers) {
	std::fstream fs(this->_path, std::fstream::in);
	size_t pos = 0;
	size_t temp_pos = 0;
	if (!fs.is_open())
		throw CustomException("CONFIG_FILE_ERROR:Invalid config filename or file is missing");
	else
	{
		this->read_file(fs);
		while (pos != this->_file_content.length())
		{
			parse_whitespace(&pos);
			if (this->_file_content.substr(pos, 6) != "server")
				throw CustomException("CONFIG_FILE_ERROR: parsing error at pos: " + std::to_string(pos) + " keyword error: " + this->_file_content.substr(pos, 6));
			else {
				pos += 6;
				this->parse_server_block(&pos, _servers);
			}
		}
		if (_servers.empty())
			throw CustomException("CONFIG_FILE_ERROR: need a server block");
	}
}