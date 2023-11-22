#include "webserv.hpp"

Location::Location() {
}

Location::~Location() {
}

std::string Location::get_route() {
	return this->_route;
}

std::string Location::get_root() {
	return this->_root;
};

std::vector<std::string> & Location::get_allowed_methods() {
	return this->_allowed_methods;
};

std::string Location::get_redirection_path() {
	return this->_redirection_path;
}

bool Location::get_autoindex() {
	return this->_autoindex;
}

std::vector<std::string> & Location::get_index() {
	return this->_index;
}

void Location::set_route(std::string route)  {
	this->_route = route;
}

void Location::set_root(std::string root) {
	this->_root = root;
}

void Location::set_redirection_path(std::string redirection_path) {
	this->_redirection_path = redirection_path;
}

void Location::set_autoindex(bool trigger) {
	this->_autoindex = trigger;
}