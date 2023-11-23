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
		std::string get_return();
		bool get_autoindex();
		std::vector<std::string> & get_index();
		void set_route(std::string route);
		void set_root(std::string root);
		void set_return(std::string _return);
		void set_autoindex(bool trigger);
	private:
		std::string _route;
		std::string _root;
		std::vector<std::string> _allowed_methods;
		std::string _return;
		bool _autoindex;
		std::vector<std::string> _index;
};

#endif