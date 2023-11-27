#include "webserv.hpp"

HTTP::HTTP() {
};

HTTP::~HTTP() {
};

void HTTP::init(const std::string path) {
	Config config(path);
	config.parse_file(this->get_servers());

	int				sockfd;
	struct addrinfo	res;

	std::vector< std::pair<std::string, std::string> >::iterator it;
	for (it = Server::address.begin(); it != Server::address.end(); it++)
	{
		std::cout << it->first << ":" << it->second << std::endl;
		sockfd = Connection::serv_listen(it->first, it->second, &res);
		Poll::add_fd(sockfd, POLLIN | POLLPRI);
	}

	Poll::put_fds();

	while (true)
	{
		if (Poll::check() == 0)
			continue;
		Poll::process(&res);
	}
	return ;
};

std::vector<Server> & HTTP::get_servers() {
	return this->_server;
}

std::ostream & operator<<(std::ostream & out, std::vector<Server> & servers) {
	size_t server_i = 0;
	size_t location_i = 0;
	size_t method_i = 0;
	size_t index_i = 0;

	while (server_i < servers.size())
	{
		out << "SERVER " + std::to_string(server_i) << std::endl;
		out << "============" << std::endl;
		out << "host: " <<servers[server_i].get_host() << std::endl;
		out << "port: " << servers[server_i].get_port() << std::endl;
		out << "server_name: " << servers[server_i].get_server_name() << std::endl;
		out << "client_max_body_size: " << servers[server_i].get_client_max_body_size() << std::endl;
		out<< "error_pages: " << std::endl;
		std::map<std::string, std::string>::iterator it = servers[server_i].get_error_pages().begin();
		for (; it != servers[server_i].get_error_pages().end(); it++) {
			out << it->first + " " << it->second << std::endl;
		}
		location_i = 0;
		std::vector<Location> & loc_vec = servers[server_i].get_location();
		while (location_i < loc_vec.size()) {
			Location & loc = loc_vec[location_i];
			out << "LOCATION: " +  std::to_string(location_i) << std::endl;
			out << "------------" << std::endl;
			out << "route: " << loc.get_route() << std::endl;
			out << "root: " << loc.get_root() << std::endl;
			out << "allowed_methods: " << std::endl;
			method_i = 0;
			std::vector<std::string> & method_vec = loc.get_allowed_methods();
			while (method_i < method_vec.size()) {
				out<< method_vec[method_i]<< std::endl;
				method_i++;
			}
			out << "index: " << std::endl;
			index_i = 0;
			std::vector<std::string> & index_vec = loc.get_index();
			while (index_i < index_vec.size()) {
				out << index_vec[index_i]<< std::endl;
				index_i++;
			}
			out << "autoindex: " << loc.get_autoindex() << std::endl;
			out << "cgi_pass: " << loc.get_cgi_pass() << std::endl;
			out << "return: " << loc.get_return() << std::endl;
			out << "------------" << std::endl;
			location_i++;
		}
		out << "============" << std::endl;
		out << std::endl;
		server_i++;
	}
	return out;
}
