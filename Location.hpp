#ifndef LOCATION_HPP
#define LOCATION_HPP
#include "webserv.hpp"

class Location {
	public:
		Location();
		~Location();
		std::string get_URI();
		std::string get_root();
		std::vector<std::string> & get_allowed_methods();
		std::string get_redirection_path();
		bool get_autoindex();
		std::vector<std::string> & get_index();
		void set_URI(std::string URI);
		void set_root(std::string root);
		void set_redirection_path(std::string redirection_path);
		void set_autoindex(bool trigger);
	private:
		std::string _URI;
		std::string _root;
		std::vector<std::string> _allowed_methods;
		std::string _redirection_path;
		bool _autoindex;
		std::vector<std::string> _index;
};

#endif