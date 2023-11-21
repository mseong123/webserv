#ifndef SERVER_HPP
#define SERVER_HPP
#include <vector>
#include <iostream>
#include "Location.hpp"

class Location;

class Server {
	public:
		std::string get_host();
		std::string get_port();
		std::string get_server_names();
		std::string get_client_max_body_size();
		std::string get_error_pages();
		std::vector<Location> & get_location();
		std::string set_host(std::string host);
		std::string set_port(std::string port);
		std::string set_server_names(std::string server_names);
		std::string set_client_max_body_size(std::string client_max_body_size);
		std::string set_error_pages(std::string error_pages);
		
	private:
		std::string _host;
		std::string _port;
		std::string _server_names;
		std::string _client_max_body_size;
		std::string _error_pages;
		std::vector<Location> _location;
};

#endif