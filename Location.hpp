#ifndef LOCATION_HPP
#define LOCATION_HPP
#include "webserv.hpp"

class Location {
	public:
		Location();
		~Location();
		std::string get_route();
		std::string get_root();
		std::vector<std::string> & get_allowed_methods();
		std::vector<std::string> & get_index();
		bool get_autoindex();
		std::string get_cgi_pass();
		std::string get_return();
		void set_route(std::string route);
		void set_root(std::string root);
		void set_autoindex(bool trigger);
		void set_cgi_pass(std::string cgi_pass);
		void set_return(std::string _return);
	private:
		std::string _route;
		std::string _root;
		std::vector<std::string> _allowed_methods;
		std::vector<std::string> _index;
		bool _autoindex;
		std::string _cgi_pass;
		std::string _return;
};

#endif