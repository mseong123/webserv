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

void Config::check_semicolon(size_t *pos, std::string keyword) {
	parse_whitespace(pos, this->_file_content);
	if (this->_file_content[*pos] == ';')
		*pos += 1;
	else
		throw CustomException("CONFIG_FILE_ERROR: no semicolon in " + keyword + " directive");
}

void Config::parse_listen(size_t *pos, Server & server) {
	size_t temp_pos = *pos;

	parse_whitespace(pos, this->_file_content);
	if (temp_pos == *pos)
		throw CustomException("CONFIG_FILE_ERROR: need whitespace after listen keyword");

	if (this->_file_content[*pos] == ';') 
		throw CustomException("CONFIG_FILE_ERROR: Need a host:name value in listen directive");
	else {
		temp_pos = this->_file_content.find_first_of(";", *pos);
		if (temp_pos != std::string::npos)
		{
			std::string host_port_value = this->_file_content.substr(*pos, temp_pos - *pos);
			size_t host_delim_pos = 0;
			if ((host_delim_pos = host_port_value.find_first_of(":")) != std::string::npos) {
				if (host_delim_pos == 0) 
					server.set_host("0.0.0.0");
				else {
					if (host_port_value.substr(0, host_delim_pos).find_first_not_of("0123456789.") != std::string::npos)
						throw CustomException("CONFIG_FILE_ERROR: Incorrect host in listen directive");
					else
						server.set_host(host_port_value.substr(0, host_delim_pos));
				}
				*pos = host_delim_pos + *pos + 1;
			}
			if (*pos == temp_pos) 
				throw CustomException("CONFIG_FILE_ERROR: Need a port value in listen directive");
			else if (this->_file_content.substr(*pos, temp_pos - *pos).find_first_not_of("0123456789") != std::string::npos)
			 	throw CustomException("CONFIG_FILE_ERROR: Incorrect port in listen directive");
			else {
				if (server.get_host() == "")
					server.set_host("0.0.0.0");
				server.set_port(this->_file_content.substr(*pos, temp_pos - *pos));
				*pos = temp_pos;
			}
		}
	}
	this->check_semicolon(pos, "listen");
}

void Config::parse_server_names(size_t *pos, Server & server) {
	size_t temp_pos = *pos;

	parse_whitespace(pos, this->_file_content);
	if (temp_pos == *pos)
		throw CustomException("CONFIG_FILE_ERROR: Need whitespace after server_names keyword");

	if (this->_file_content[*pos] == ';') 
		throw CustomException("CONFIG_FILE_ERROR: Need a value in server_names directive");
	else {
		temp_pos = this->_file_content.find_first_of(";", *pos);
		if (temp_pos != std::string::npos)
		{
			if (this->_file_content.substr(*pos, temp_pos - *pos).find_first_not_of("abcdefghijklmnopqrstuvwxyz0123456789._") != std::string::npos)
				throw CustomException("CONFIG_FILE_ERROR: Incorrect server_names value");
			else {
				server.set_server_names(this->_file_content.substr(*pos, temp_pos - *pos));
				*pos = temp_pos;
			}
		}
	}
	this->check_semicolon(pos, "server_names");
}

void Config::parse_client_max_body_size(size_t *pos, Server & server) {
	size_t temp_pos = *pos;

	parse_whitespace(pos, this->_file_content);
	if (temp_pos == *pos)
		throw CustomException("CONFIG_FILE_ERROR: Need whitespace after client_max_body_size keyword");

	if (this->_file_content[*pos] == ';') 
		throw CustomException("CONFIG_FILE_ERROR: Need a client_max_body_size value");
	else {
		temp_pos = this->_file_content.find_first_of(";", *pos);
		if (temp_pos != std::string::npos)
		{
			if (this->_file_content.substr(*pos, temp_pos - *pos).find_first_not_of("0123456789") != std::string::npos)
				throw CustomException("CONFIG_FILE_ERROR: Insert only integer values in client_max_body_size");
			else {
				server.set_client_max_body_size(this->_file_content.substr(*pos, temp_pos - *pos));
				*pos = temp_pos;
			}
		}
		this->check_semicolon(pos, "client_max_body_size");
	}
}

void Config::parse_error_pages(size_t *pos, Server & server) {
	size_t temp_pos = *pos;
	std::pair<std::string, std::string> error_page;

	parse_whitespace(pos, this->_file_content);
	if (temp_pos == *pos)
		throw CustomException("CONFIG_FILE_ERROR: Need whitespace after error_pages keyword");
	
	if (this->_file_content[*pos] == ';')
		throw CustomException("CONFIG_FILE_ERROR: Need custom error_pages value");
	else {
		temp_pos = this->_file_content.find_first_of(";", *pos);
		if (temp_pos != std::string::npos)
		{
			std::string error_pages_string = this->_file_content.substr(*pos, temp_pos - *pos);
			std::string error_pages_value;
			std::string error_pages_directory;
			size_t delim_pos = 0;
			if ((delim_pos = error_pages_string.find_first_of(" \t")) == std::string::npos)
				throw CustomException("CONFIG_FILE_ERROR: Incorrect custom error_pages format. Correct format ie 404 /404.html;");
			else {
				if ((error_pages_value = error_pages_string.substr(0, delim_pos)).find_first_not_of("0123456789") != std::string::npos || error_pages_value.length() != 3)
						throw CustomException("CONFIG_FILE_ERROR: Incorrect custom error_pages value");
				else {
					parse_whitespace(&delim_pos, error_pages_string);
					if ((error_pages_directory = error_pages_string.substr(delim_pos, std::string::npos)).find_first_of(" \t") != std::string::npos || error_pages_directory[0] != '/')
							throw CustomException("CONFIG_FILE_ERROR: Incorrect custom error_pages directory value");
					else {
						error_page.first = error_pages_value;
						error_page.second = error_pages_directory;
						server.get_error_pages().insert(error_page);
						*pos += error_pages_string.length();
					}
				}
			}
		}
		this->check_semicolon(pos, "error_pages");
	}
}

void Config::parse_allowed_methods(size_t *pos, Location & location) {
	size_t temp_pos = *pos;

	parse_whitespace(pos, this->_file_content);
	if (temp_pos == *pos)
		throw CustomException("CONFIG_FILE_ERROR: Need whitespace after allowed_methods keyword");

	if ((temp_pos = this->_file_content.find_first_not_of(" ;\t}", *pos)) == std::string::npos) 
		throw CustomException("CONFIG_FILE_ERROR: Need one allowed_methods value");
	else {
		*pos = temp_pos;
		temp_pos = this->_file_content.find_first_of(" ;\t}", *pos);
		if (temp_pos != std::string::npos)
		{
			std::string allowed_method = this->_file_content.substr(*pos, temp_pos - *pos);
			std::cout << allowed_method << std::endl;
			if (allowed_method == "POST" || allowed_method == "GET" || allowed_method == "DELETE")
			{
				location.get_allowed_methods().push_back(allowed_method);
				*pos = temp_pos;
			}
			else
				throw CustomException("CONFIG_FILE_ERROR: Incorrect allowed_methods value");
		}
		this->check_semicolon(pos, "allowed_methods");
	}
	
}

void Config::parse_location_block(size_t *pos, std::vector<Location> & _location) {
	size_t temp_pos = 0;
	bool closing_bracket = false;
	Location location;
	
	parse_whitespace(pos, this->_file_content);
	if (this->_file_content[*pos] != '/') 
		throw CustomException("CONFIG_FILE_ERROR: Incorrect route in location directive");
	else {
		temp_pos = this->_file_content.find_first_of(" \t{", *pos);
		if (temp_pos != std::string::npos)
			location.set_route(this->_file_content.substr(*pos, temp_pos - *pos));
		*pos = temp_pos;
	}

	parse_whitespace(pos, this->_file_content);
	if (this->_file_content[*pos] != '{') 
		throw CustomException("CONFIG_FILE_ERROR: Syntax error: missing { in location block");
	else
		*pos += 1;

	while (*pos != this->_file_content.length()) {
		parse_whitespace(pos, this->_file_content);
		if (this->_file_content.substr(*pos, 15) == "allowed_methods") {
			*pos += 15;
			this->parse_allowed_methods(pos, location);
		}
		else if (this->_file_content[*pos] == '}') {
			*pos += 1;
			break;
		}
		else {
			throw CustomException("CONFIG_FILE_ERROR: Incorrect directive in location block or missing } in location block");
		}
	}
	_location.push_back(location);
	std::cout<<"size"<<_location.size()<<std::endl;
		
}

void Config::parse_server_block(size_t *pos, std::vector<Server> & _server) {
	bool closing_bracket = false;
	Server server;

	parse_whitespace(pos, this->_file_content);
	if (this->_file_content[*pos] != '{') 
		throw CustomException("CONFIG_FILE_ERROR: Syntax error: missing { in server block");
	else
		*pos += 1;

	while (*pos != this->_file_content.length()) {
		parse_whitespace(pos, this->_file_content);
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
		else if (this->_file_content.substr(*pos, 1) == "}") {
			*pos += 1;
			closing_bracket = true;
			break;
		}
		else if (*pos != this->_file_content.length()) {
			throw CustomException("CONFIG_FILE_ERROR: Incorrect directive in server block");
		}
		
	}
	
	if (!closing_bracket)
		throw CustomException("CONFIG_FILE_ERROR: missing } in server block");
	_server.push_back(server);
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
			parse_whitespace(&pos, this->_file_content);
			if (pos != this->_file_content.length())
			{
				if (this->_file_content.substr(pos, 6) != "server")
					throw CustomException("CONFIG_FILE_ERROR: server keyword error: " + this->_file_content.substr(pos, 6));
				else {
					pos += 6;
					this->parse_server_block(&pos, _servers);
				}
			}
		}
		if (_servers.empty())
			throw CustomException("CONFIG_FILE_ERROR: need a server block");
		size_t i = 0;
		while (i < _servers.size())
		{
		std::cout<<_servers[i].get_host() << std::endl;
		std::cout<<_servers[i].get_port() << std::endl;
		std::cout<<_servers[i].get_server_names() << std::endl;
		std::cout<<_servers[i].get_error_pages().find("404")->second << std::endl;
		i++;
		}
	}
}