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

void Config::parse_whitespace(size_t *pos) {
	size_t temp_pos = this->_file_content.find_first_not_of(" \t", *pos);
	if (temp_pos != std::string::npos)
		*pos = temp_pos;
}

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
	parse_whitespace(pos);
	if (this->_file_content.find_first_of("0123456789", *pos) == std::string::npos) {
		throw CustomException("CONFIG_FILE_ERROR: Need a host:name value in listen directive");
	}
	else {
		if (server.get_host() == "")
			server.set_host("0.0.0.0");
		temp_pos = this->_file_content.find_first_of(" ;\t}", *pos);
		if (temp_pos != std::string::npos)
		{
			if (this->_file_content.substr(*pos, temp_pos-* pos).find_first_not_of("0123456789") != std::string::npos)
				throw CustomException("CONFIG_FILE_ERROR: Incorrect port value");
			server.set_port(this->_file_content.substr(*pos, temp_pos - *pos));
			*pos = temp_pos;
		}
		this->check_semicolon(pos, "listen");
	}
}

void Config::parse_server_names(size_t *pos, Server & server) {
	size_t temp_pos = *pos;

	parse_whitespace(pos);
	if (temp_pos == *pos)
		throw CustomException("CONFIG_FILE_ERROR: Need whitespace after server_names keyword");
	if ((temp_pos = this->_file_content.find_first_not_of(" ;\t}", *pos)) == std::string::npos) 
		throw CustomException("CONFIG_FILE_ERROR: Need a server_names value");
	else {
		*pos = temp_pos;
		temp_pos = this->_file_content.find_first_of(" ;\t}", *pos);
		if (temp_pos != std::string::npos)
		{
			server.set_server_names(this->_file_content.substr(*pos, temp_pos - *pos));
			*pos = temp_pos;
		}
		this->check_semicolon(pos, "server_names");
	}
}

void Config::parse_client_max_body_size(size_t *pos, Server & server) {
	size_t temp_pos = *pos;

	parse_whitespace(pos);
	if (temp_pos == *pos)
		throw CustomException("CONFIG_FILE_ERROR: Need whitespace after client_max_body_size keyword");
	if ((temp_pos = this->_file_content.find_first_not_of(" ;\t}", *pos)) == std::string::npos) 
		throw CustomException("CONFIG_FILE_ERROR: Need a client_max_body_size value");
	else {
		*pos = temp_pos;
		temp_pos = this->_file_content.find_first_of(" ;\t}", *pos);
		if (temp_pos != std::string::npos)
		{
			if (this->_file_content.substr(*pos, temp_pos - *pos).find_first_not_of("0123456789") != std::string::npos)
				throw CustomException("CONFIG_FILE_ERROR: Insert only integer values in client_max_body_size");
			server.set_client_max_body_size(this->_file_content.substr(*pos, temp_pos - *pos));
			*pos = temp_pos;
		}
		this->check_semicolon(pos, "client_max_body_size");
	}
}

void Config::parse_error_pages(size_t *pos, Server & server) {
	size_t temp_pos = *pos;
	std::pair<std::string, std::string> error_page;

	parse_whitespace(pos);
	if (temp_pos == *pos)
		throw CustomException("CONFIG_FILE_ERROR: Need whitespace after error_pages keyword");
	
	if ((temp_pos = this->_file_content.find_first_not_of(" ;\t}", *pos)) == std::string::npos) 
		throw CustomException("CONFIG_FILE_ERROR: Need custom error_pages value");
	else {
		*pos = temp_pos;
		temp_pos = this->_file_content.find_first_of(" ;\t}", *pos);
		if (temp_pos != std::string::npos)
		{
			if (this->_file_content.substr(*pos, temp_pos - *pos).find_first_not_of("0123456789") != std::string::npos || (temp_pos - *pos) != 3)
				throw CustomException("CONFIG_FILE_ERROR: Incorrect custom error_pages value");
			error_page.first = this->_file_content.substr(*pos, temp_pos - *pos);
			*pos = temp_pos;
		}
	}
	if ((temp_pos = this->_file_content.find_first_not_of(" ;\t}", *pos)) == std::string::npos) 
		throw CustomException("CONFIG_FILE_ERROR: Need custom error_pages directory");
	else {
		*pos = temp_pos;
		temp_pos = this->_file_content.find_first_of(" ;\t}", *pos);
		if (temp_pos != std::string::npos)
		{
			if (this->_file_content.substr(*pos, temp_pos - *pos).find_first_not_of("/0123456789.abcdefghijklmnopqrstuvwxyz") != std::string::npos || this->_file_content[*pos] != '/')
				throw CustomException("CONFIG_FILE_ERROR: Incorrect custom error_pages directory, need to start with '/'");
			error_page.second = this->_file_content.substr(*pos, temp_pos - *pos);
			*pos = temp_pos;
		}
		this->check_semicolon(pos, "error_pages");
	}
	server.get_error_pages().insert(error_page);
}

void Config::parse_location_block(size_t *pos, std::vector<Location> & _location) {
	size_t temp_pos = 0;
	bool closing_bracket = false;
	Location location;
	_location.push_back(location);

	parse_whitespace(pos);
	if (this->_file_content[*pos] != '/') 
		throw CustomException("CONFIG_FILE_ERROR: Incorrect route in location directive");
	else {
		temp_pos = this->_file_content.find_first_of(" \t{", *pos);
		if (temp_pos != std::string::npos) {
			location.set_route(this->_file_content.substr(*pos, temp_pos - *pos));
		}
		*pos = temp_pos;
	}

	parse_whitespace(pos);
	if (this->_file_content[*pos] != '{') 
		throw CustomException("CONFIG_FILE_ERROR: Syntax error: missing { in location block");
	else
		*pos += 1;
	while (*pos != this->_file_content.length()) {
		
	}
		
}


void Config::parse_server_block(size_t *pos, std::vector<Server> & _server) {
	bool closing_bracket = false;
	Server server;
	_server.push_back(server);

	parse_whitespace(pos);
	if (this->_file_content[*pos] != '{') 
		throw CustomException("CONFIG_FILE_ERROR: Syntax error: missing { in server block");
	else
		*pos += 1;

	while (*pos != this->_file_content.length()) {
		parse_whitespace(pos);
		if (this->_file_content.substr(*pos, 6) == "listen") {
			if (server.get_host() != "")
				throw CustomException("CONFIG_FILE_ERROR: Only 1 listen directive allowed in server block");
			else {
			*pos += 6;
			this->parse_listen(pos, server);
			}
		}
		else if (this->_file_content.substr(*pos, 12) == "server_names") {
			if (server.get_server_names() != "")
				throw CustomException("CONFIG_FILE_ERROR: Only 1 server_names directive allowed in server block");
			else {
				*pos += 12;
				this->parse_server_names(pos, server);
			}
		}
		else if (this->_file_content.substr(*pos, 20) == "client_max_body_size") {
			if (server.get_client_max_body_size() != "")
				throw CustomException("CONFIG_FILE_ERROR: Only 1 client_max_body_size directive allowed in server block");
			else {
				*pos += 20;
				this->parse_client_max_body_size(pos, server);
			}
		}
		else if (this->_file_content.substr(*pos, 11) == "error_pages") {
			*pos += 11;
			this->parse_error_pages(pos, server);
		}
		else if (this->_file_content.substr(*pos, 8) == "location") {
			*pos += 8;
			this->parse_location_block(pos, server.get_location());
		}
		else if (this->_file_content[*pos] == '}') {
			*pos += 1;
			closing_bracket = true;
			break;
		}
		else {
			throw CustomException("CONFIG_FILE_ERROR: Incorrect directive in server block");
		}
	}
	if (!closing_bracket)
		throw CustomException("CONFIG_FILE_ERROR: Syntax error: missing } in server block");

	std::cout << server.get_host() << std::endl;
	std::cout << server.get_port() << std::endl;
	std::cout << server.get_server_names() << std::endl;
	std::cout << server.get_client_max_body_size() << std::endl;
	std::cout << server.get_error_pages().find("405")->second << std::endl;
	std::cout << server.get_error_pages().find("404")->second << std::endl;

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
				throw CustomException("CONFIG_FILE_ERROR: keyword error: " + this->_file_content.substr(pos, 6));
			else {
				pos += 6;
				this->parse_server_block(&pos, _servers);
			}
		}
		if (_servers.empty())
			throw CustomException("CONFIG_FILE_ERROR: need a server block");
	}
}