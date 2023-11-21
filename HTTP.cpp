#include "webserv.hpp"

HTTP::HTTP() {
};

HTTP::~HTTP() {
};

void HTTP::init(const std::string path) {
	Config config(path);
	config.parse_file(this->get_servers());

};

std::vector<Server> & HTTP::get_servers() {
	return this->_servers;
}