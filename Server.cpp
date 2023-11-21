#include "webserv.hpp"

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

void Server::set_host(std::string host) {
	this->_host = host;
}

void Server::set_port(std::string port) {
	this->_port = port;
}

void Server::set_server_names(std::string server_names) {
	this->_server_names = server_names;
}

void Server::set_client_max_body_size(std::string client_max_body_size) {
	this->_client_max_body_size = client_max_body_size;
}

void Server::set_error_pages(std::string error_pages) {
	this->_error_pages = error_pages;
}

