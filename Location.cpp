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

std::string Location::get_return() {
	return this->_return;
}

bool Location::get_autoindex() {
	return this->_autoindex;
}

std::vector<std::string> & Location::get_index() {
	return this->_index;
}

std::string Location::get_cgi_pass() {
	return this->_cgi_pass;
}

void Location::set_route(std::string route)  {
	this->_route = route;
}

void Location::set_root(std::string root) {
	this->_root = root;
}

void Location::set_return(std::string _return) {
	this->_return = _return;
}

void Location::set_autoindex(bool trigger) {
	this->_autoindex = trigger;
}

void Location::set_cgi_pass(std::string cgi_pass) {
	this->_cgi_pass = cgi_pass;
}