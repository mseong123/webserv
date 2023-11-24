#include "webserv.hpp"

Server::Server() {
}

Server::~Server(){
}

std::string Server::get_host() const{
	return this->_host;
}

std::string Server::get_port() const{
	return this->_port;
}

std::string Server::get_server_name() {
	return this->_server_name;
}
		
std::string Server::get_client_max_body_size() {
	return this->_client_max_body_size;
}

std::map<std::string, std::string> & Server::get_error_pages() {
	return this->_error_pages;
}

std::vector<Location> & Server::get_location() {
	return this->_location;
}

void Server::set_host(std::string host) {
	this->_host = host;
}

void Server::set_port(std::string port) {
	this->_port = port;
}

void Server::set_server_name(std::string server_name) {
	this->_server_name = server_name;
}

void Server::set_client_max_body_size(std::string client_max_body_size) {
	this->_client_max_body_size = client_max_body_size;
}

