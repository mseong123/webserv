#include "Server.hpp"

Server::Server(){
}

Server::~Server(){
}

std::string Server::get_host(){
	return this->_host;
}

std::string Server::get_port() {
	return this->_port;
}

std::string Server::get_server_names() {
	return this->_server_names;
}
		
std::string Server::get_client_max_body_size() {
	return this->_client_max_body_size;
}

std::string Server::get_error_pages() {
	return this->_error_pages;
}

std::vector<Location> & Server::get_location() {
	return this->_location;
}

std::string Server::set_host(std::string host) {
	this->_host = host;
}

std::string Server::set_port(std::string port) {
	this->_port = port;
}

std::string Server::set_server_names(std::string server_names) {
	this->_server_names = server_names;
}

std::string set_client_max_body_size(std::string client_max_body_size);
	std::string set_error_pages();

