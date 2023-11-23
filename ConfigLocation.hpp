#ifndef CONFIGLOCATION_HPP
#define CONFIGLOCATION_HPP

class ConfigLocation {
	public:
		ConfigLocation();
		~ConfigLocation();
		void parse_whitespace(size_t *pos, std::string content);
		void check_semicolon(size_t *pos, std::string keyword, std::string file_content);
		void parse_allowed_methods(size_t *pos, Location & location, std::string file_content);
		void parse_return(size_t *pos, Location & location, std::string file_content);
		void parse_root(size_t *pos, Location & location, std::string file_content);
		void parse_cgi_pass(size_t *pos, Location & location, std::string file_content);
		void parse_autoindex(size_t *pos, Location & location, std::string file_content);
		void parse_index(size_t *pos, Location & location, std::string file_content);
		void parse_location_block(size_t *pos, std::vector<Location> & _location, std::string file_content);
};

#endif