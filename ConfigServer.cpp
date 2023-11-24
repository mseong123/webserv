#include "webserv.hpp"

ConfigServer::ConfigServer() {
}

ConfigServer::~ConfigServer() {
}

void ConfigServer::parse_whitespace(size_t *pos, std::string content) {
	while (*pos != content.length()) 
	{
		if (content[*pos] == '\t' || content[*pos] == ' ')
			*pos += 1;
		else
			break;
	}
}

void ConfigServer::check_semicolon(size_t *pos, std::string keyword, std::string file_content) {
	parse_whitespace(pos, file_content);
	if (file_content[*pos] == ';')
		*pos += 1;
	else
		throw CustomException("CONFIG_FILE_ERROR: no semicolon in " + keyword + " directive");
}

void ConfigServer::parse_listen(size_t *pos, Server & server, std::string file_content) {
	size_t temp_pos = *pos;

	parse_whitespace(pos, file_content);
	if (temp_pos == *pos)
		throw CustomException("CONFIG_FILE_ERROR: need whitespace after listen keyword");

	if (file_content[*pos] == ';') 
		throw CustomException("CONFIG_FILE_ERROR: Need a host:name value in listen directive");
	else {
		temp_pos = file_content.find_first_of(";", *pos);
		if (temp_pos != std::string::npos)
		{
			std::string host_port_value = file_content.substr(*pos, temp_pos - *pos);
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
			else if (file_content.substr(*pos, temp_pos - *pos).find_first_not_of("0123456789") != std::string::npos)
			 	throw CustomException("CONFIG_FILE_ERROR: Incorrect port in listen directive");
			else {
				if (server.get_host() == "")
					server.set_host("0.0.0.0");
				server.set_port(file_content.substr(*pos, temp_pos - *pos));
				*pos = temp_pos;
			}
		}
	}
	this->check_semicolon(pos, "listen", file_content);
}

void ConfigServer::parse_server_name(size_t *pos, Server & server, std::string file_content) {
	size_t temp_pos = *pos;

	parse_whitespace(pos, file_content);
	if (temp_pos == *pos)
		throw CustomException("CONFIG_FILE_ERROR: Need whitespace after server_name keyword");

	if (file_content[*pos] == ';') 
		throw CustomException("CONFIG_FILE_ERROR: Need a value in server_name directive");
	else {
		temp_pos = file_content.find_first_of(";", *pos);
		if (temp_pos != std::string::npos)
		{
			if (file_content.substr(*pos, temp_pos - *pos).find_first_not_of("abcdefghijklmnopqrstuvwxyz0123456789._") != std::string::npos)
				throw CustomException("CONFIG_FILE_ERROR: Incorrect server_name value");
			else {
				server.set_server_name(file_content.substr(*pos, temp_pos - *pos));
				*pos = temp_pos;
			}
		}
	}
	this->check_semicolon(pos, "server_name", file_content);
}

void ConfigServer::parse_client_max_body_size(size_t *pos, Server & server, std::string file_content) {
	size_t temp_pos = *pos;

	parse_whitespace(pos, file_content);
	if (temp_pos == *pos)
		throw CustomException("CONFIG_FILE_ERROR: Need whitespace after client_max_body_size keyword");

	if (file_content[*pos] == ';') 
		throw CustomException("CONFIG_FILE_ERROR: Need a client_max_body_size value");
	else {
		temp_pos = file_content.find_first_of(";", *pos);
		if (temp_pos != std::string::npos)
		{
			if (file_content.substr(*pos, temp_pos - *pos).find_first_not_of("0123456789") != std::string::npos)
				throw CustomException("CONFIG_FILE_ERROR: Insert only integer values in client_max_body_size");
			else {
				server.set_client_max_body_size(file_content.substr(*pos, temp_pos - *pos));
				*pos = temp_pos;
			}
		}
		this->check_semicolon(pos, "client_max_body_size", file_content);
	}
}

void ConfigServer::parse_error_pages(size_t *pos, Server & server, std::string file_content) {
	size_t temp_pos = *pos;
	std::pair<std::string, std::string> error_page;

	parse_whitespace(pos, file_content);
	if (temp_pos == *pos)
		throw CustomException("CONFIG_FILE_ERROR: Need whitespace after error_pages keyword");
	
	if (file_content[*pos] == ';')
		throw CustomException("CONFIG_FILE_ERROR: Need custom error_pages value");
	else {
		temp_pos = file_content.find_first_of(";", *pos);
		if (temp_pos != std::string::npos)
		{
			std::string error_pages_string = file_content.substr(*pos, temp_pos - *pos);
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
		this->check_semicolon(pos, "error_pages", file_content);
	}
}

void ConfigServer::parse_server_block(size_t *pos, std::vector<Server> & _server, std::string file_content) {
	bool closing_bracket = false;
	Server server;

	parse_whitespace(pos, file_content);
	if (file_content[*pos] != '{') 
		throw CustomException("CONFIG_FILE_ERROR: Syntax error: missing { in server block");
	else
		*pos += 1;

	while (*pos != file_content.length()) {
		parse_whitespace(pos, file_content);
		if (file_content.substr(*pos, 6) == "listen") {
				*pos += 6;
				this->parse_listen(pos, server, file_content);
			}
		else if (file_content.substr(*pos, 12) == "server_name") {
				*pos += 12;
				this->parse_server_name(pos, server, file_content);
			}
		else if (file_content.substr(*pos, 20) == "client_max_body_size") {
				*pos += 20;
				this->parse_client_max_body_size(pos, server, file_content);
		}
		else if (file_content.substr(*pos, 11) == "error_pages") {
			*pos += 11;
			this->parse_error_pages(pos, server, file_content);
		}
		else if (file_content.substr(*pos, 8) == "location") {
			ConfigLocation config_location;
			*pos += 8;
			config_location.parse_location_block(pos, server.get_location(), file_content);
		}
		else if (file_content.substr(*pos, 1) == "}") {
			*pos += 1;
			closing_bracket = true;
			break;
		}
		else if (*pos != file_content.length()) {
			throw CustomException("CONFIG_FILE_ERROR: Incorrect directive in server block");
		}
	}
	
	if (!closing_bracket)
		throw CustomException("CONFIG_FILE_ERROR: missing } in server block");
	_server.push_back(server);
}

