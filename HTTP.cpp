#include "webserv.hpp"

HTTP::HTTP() {
};

HTTP::~HTTP() {
};

void HTTP::init(const std::string path) {
	Config config(path);
	config.parse_file(this->get_server());
	Server server = this->_server[0];
	std::cout<<"size"<<server.get_host()<< std::endl;

};

std::vector<Server> & HTTP::get_server() {
	return this->_server;
}