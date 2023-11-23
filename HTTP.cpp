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

void HTTP::print_servers() {
	size_t server_i = 0;
	size_t location_i = 0;
	size_t method_i = 0;
	size_t index_i = 0;

	while (server_i < _servers.size())
	{
		std::cout << "SERVER " + std::to_string(server_i) << std::endl;
		std::cout << "============" << std::endl;
		std::cout<< "host: " <<_servers[server_i].get_host() << std::endl;
		std::cout<< "port: " << _servers[server_i].get_port() << std::endl;
		std::cout<< "server_names: " <<_servers[server_i].get_server_names() << std::endl;
		std::cout<< "client_max_body_size: " <<_servers[server_i].get_client_max_body_size() << std::endl;
		std::cout<< "error_pages: " << std::endl;
		std::map<std::string, std::string>::iterator it = _servers[server_i].get_error_pages().begin();
		for (; it != _servers[server_i].get_error_pages().end(); it++) {
			std::cout << it->first + " " << it->second << std::endl;
		}
		location_i = 0;
		std::vector<Location> & loc_vec = _servers[server_i].get_location();
		while (location_i < loc_vec.size()) {
			Location & loc = loc_vec[location_i];
			std::cout << "LOCATION: " +  std::to_string(location_i) << std::endl;
			std::cout << "------------" << std::endl;
			std::cout<< "route: " << loc.get_route() << std::endl;
			std::cout<< "root: " << loc.get_root() << std::endl;
			std::cout<< "allowed_methods: " << std::endl;
			method_i = 0;
			std::vector<std::string> & method_vec = loc.get_allowed_methods();
			while (method_i < method_vec.size()) {
				std::cout<< method_vec[method_i]<< std::endl;
				method_i++;
			}
			std::cout<< "index: " << std::endl;
			index_i = 0;
			std::vector<std::string> & index_vec = loc.get_index();
			while (index_i < index_vec.size()) {
				std::cout << index_vec[index_i]<< std::endl;
				index_i++;
			}
			std::cout<< "autoindex: " << loc.get_autoindex() << std::endl;
			std::cout<< "cgi_pass: " << loc.get_cgi_pass() << std::endl;
			std::cout<< "return: " << loc.get_return() << std::endl;
			std::cout << "------------" << std::endl;
			location_i++;
		}
		std::cout << "============" << std::endl;
		std::cout << std::endl;
		server_i++;
	}
}
