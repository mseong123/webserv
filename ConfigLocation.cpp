#include "webserv.hpp"

ConfigLocation::ConfigLocation() {
}

ConfigLocation::~ConfigLocation() {
}

void ConfigLocation::parse_whitespace(size_t *pos, std::string content) {
	while (*pos != content.length()) 
	{
		if (content[*pos] == '\t' || content[*pos] == ' ')
			*pos += 1;
		else
			break;
	}
}

void ConfigLocation::check_semicolon(size_t *pos, std::string keyword, std::string file_content) {
	parse_whitespace(pos, file_content);
	if (file_content[*pos] == ';')
		*pos += 1;
	else
		throw CustomException("CONFIG_FILE_ERROR: no semicolon in " + keyword + " directive");
}

void ConfigLocation::parse_allowed_methods(size_t *pos, Location & location, std::string file_content) {
	size_t temp_pos = *pos;

	parse_whitespace(pos, file_content);
	if (temp_pos == *pos)
		throw CustomException("CONFIG_FILE_ERROR: Need whitespace after allowed_methods keyword");

	if (file_content[*pos] == ';') 
		throw CustomException("CONFIG_FILE_ERROR: Need a value in allowed_methods directive");
	else {
		temp_pos = file_content.find_first_of(";", *pos);
		if (temp_pos != std::string::npos)
		{
			std::string method = file_content.substr(*pos, temp_pos - *pos);
			if (method == "GET" || method == "POST" || method == "DELETE") {
				std::vector<std::string>::iterator it;
				std::vector<std::string> & method_vec = location.get_allowed_methods();
				it = std::find(method_vec.begin(), method_vec.end(), method);
				if (it == method_vec.end())
					method_vec.push_back(method);
				*pos = temp_pos;
			}
			else
				throw CustomException("CONFIG_FILE_ERROR: Incorrect value in allowed_methods directive (use only GET, POST and DELETE methods)");
		}
	}
	this->check_semicolon(pos, "allowed_methods", file_content);
}

void ConfigLocation::parse_return(size_t *pos, Location & location, std::string file_content) {
	size_t temp_pos = *pos;

	parse_whitespace(pos, file_content);
	if (temp_pos == *pos)
		throw CustomException("CONFIG_FILE_ERROR: Need whitespace after return keyword");

	if (file_content[*pos] == ';') 
		throw CustomException("CONFIG_FILE_ERROR: Need a value in return directive");
	else {
		temp_pos = file_content.find_first_of(";", *pos);
		if (temp_pos != std::string::npos)
		{
			
			std::string path = file_content.substr(*pos, temp_pos - *pos);
			if (path.find_first_of(" \t") != std::string::npos)
				throw CustomException("CONFIG_FILE_ERROR: Incorrect value in return directive");
			else {
				location.set_return(path);
				*pos = temp_pos;
			}
		}
	}
	this->check_semicolon(pos, "return", file_content);
}

void ConfigLocation::parse_autoindex(size_t *pos, Location & location, std::string file_content) {
	size_t temp_pos = *pos;

	parse_whitespace(pos, file_content);
	if (temp_pos == *pos)
		throw CustomException("CONFIG_FILE_ERROR: Need whitespace after autoindex keyword");

	if (file_content[*pos] == ';') 
		throw CustomException("CONFIG_FILE_ERROR: Need a value in autoindex directive");
	else {
		temp_pos = file_content.find_first_of(";", *pos);
		if (temp_pos != std::string::npos)
		{
			std::string autoindex = file_content.substr(*pos, temp_pos - *pos);
			if (autoindex == "on" || autoindex == "off") {
				autoindex == "on"? location.set_autoindex(true) : location.set_autoindex(false);
				*pos = temp_pos;
			}
			else
				throw CustomException("CONFIG_FILE_ERROR: Incorrect value in autoindex directive (use only 'on' or 'off')");
		
		}
	}
	this->check_semicolon(pos, "autoindex", file_content);
}

void ConfigLocation::parse_root(size_t *pos, Location & location, std::string file_content) {
	size_t temp_pos = *pos;

	parse_whitespace(pos, file_content);
	if (temp_pos == *pos)
		throw CustomException("CONFIG_FILE_ERROR: Need whitespace after root keyword");

	if (file_content[*pos] == ';') 
		throw CustomException("CONFIG_FILE_ERROR: Need a value in root directive");
	else {
		temp_pos = file_content.find_first_of(";", *pos);
		if (temp_pos != std::string::npos)
		{
			std::string path = file_content.substr(*pos, temp_pos - *pos);
			if (path.find_first_of(" \t") != std::string::npos || path[0] != '/')
				throw CustomException("CONFIG_FILE_ERROR: Incorrect value in root directive (need to start with /)");
			else {
				location.set_root(path);
				*pos = temp_pos;
			}
		}
	}
	this->check_semicolon(pos, "root", file_content);
}

void ConfigLocation::parse_index(size_t *pos, Location & location, std::string file_content) {
	size_t temp_pos = *pos;

	parse_whitespace(pos, file_content);
	if (temp_pos == *pos)
		throw CustomException("CONFIG_FILE_ERROR: Need whitespace after index keyword");

	if (file_content[*pos] == ';') 
		throw CustomException("CONFIG_FILE_ERROR: Need a value in index directive");
	else {
		temp_pos = file_content.find_first_of(";", *pos);
		if (temp_pos != std::string::npos)
		{
			std::string index = file_content.substr(*pos, temp_pos - *pos);
			if (index.find_first_of(" \t") != std::string::npos)
				throw CustomException("CONFIG_FILE_ERROR: Incorrect value in index directive");
			else {
				std::vector<std::string>::iterator it;
				std::vector<std::string> & index_vec = location.get_index();
				it = std::find(index_vec.begin(), index_vec.end(), index);
				if (it == index_vec.end())
					index_vec.push_back(index);
				*pos = temp_pos;
			}
		}
	}
	this->check_semicolon(pos, "index", file_content);
}

void ConfigLocation::parse_cgi_pass(size_t *pos, Location & location, std::string file_content) {
	size_t temp_pos = *pos;

	parse_whitespace(pos, file_content);
	if (temp_pos == *pos)
		throw CustomException("CONFIG_FILE_ERROR: Need whitespace after cgi_pass keyword");

	if (file_content[*pos] == ';') 
		throw CustomException("CONFIG_FILE_ERROR: Need a value in cgi_pass directive");
	else {
		temp_pos = file_content.find_first_of(";", *pos);
		if (temp_pos != std::string::npos)
		{
			std::string path = file_content.substr(*pos, temp_pos - *pos);
			if (path.find_first_of(" \t") != std::string::npos)
				throw CustomException("CONFIG_FILE_ERROR: Incorrect value in cgi_pass directive");
			else {
				location.set_cgi_pass(path);
				*pos = temp_pos;
			}
		}
	}
	this->check_semicolon(pos, "cgi_pass", file_content);
}

void ConfigLocation::parse_location_block(size_t *pos, std::vector<Location> & _location, std::string file_content) {
	size_t temp_pos = 0;
	bool closing_bracket = false;
	Location location;
	
	parse_whitespace(pos, file_content);
	if (file_content[*pos] != '/') 
		throw CustomException("CONFIG_FILE_ERROR: Incorrect route in location block (need to start with /)");
	else {
		temp_pos = file_content.find_first_of(" \t{", *pos);
		if (temp_pos != std::string::npos)
			location.set_route(file_content.substr(*pos, temp_pos - *pos));
		*pos = temp_pos;
	}
	parse_whitespace(pos, file_content);
	if (file_content[*pos] != '{') 
		throw CustomException("CONFIG_FILE_ERROR: Syntax error: missing { in location block");
	else
		*pos += 1;

	while (*pos != file_content.length()) {
		parse_whitespace(pos, file_content);
		if (file_content.substr(*pos, 15) == "allowed_methods") {
			*pos += 15;
			this->parse_allowed_methods(pos, location, file_content);
		}
		else if (file_content.substr(*pos, 6) == "return") {
			*pos += 6;
			this->parse_return(pos, location, file_content);
		}
		else if (file_content.substr(*pos, 4) == "root") {
			*pos += 4;
			this->parse_root(pos, location, file_content);
		}
		else if (file_content.substr(*pos, 8) == "cgi_pass") {
			*pos += 8;
			this->parse_cgi_pass(pos, location, file_content);
		}
		else if (file_content.substr(*pos, 9) == "autoindex") {
			*pos += 9;
			this->parse_autoindex(pos, location, file_content);
		}
		else if (file_content.substr(*pos, 5) == "index") {
			*pos += 5;
			this->parse_index(pos, location, file_content);
		}
		else if (file_content[*pos] == '}') {
			*pos += 1;
			closing_bracket = true;
			break;
		}
		else if (*pos != file_content.length()) {
			throw CustomException("CONFIG_FILE_ERROR: Incorrect directive in location block");
		}
	}
	if (!closing_bracket)
		throw CustomException("CONFIG_FILE_ERROR: missing } in location block");
	_location.push_back(location);
}