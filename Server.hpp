#ifndef SERVER_HPP
#define SERVER_HPP
#include "webserv.hpp"

class Location;

class Server {
	public:
		Server();
		~Server();
		std::string get_host() const;
		std::string get_port() const;
		std::string get_server_name();
		std::string get_client_max_body_size();
		std::map<std::string, std::string> & get_error_pages();
		std::vector<Location> & get_location();
		void set_host(std::string host);
		void set_port(std::string port);
		void set_server_name(std::string server_name);
		void set_client_max_body_size(std::string client_max_body_size);
		
	private:
		std::string _host;
		std::string _port;
		std::string _server_name;
		std::string _client_max_body_size;
		std::map<std::string, std::string> _error_pages;
		std::vector<Location> _location;
};

#endif