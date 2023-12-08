#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "webserv.hpp"

class	Response {
	public:
		Response(void);
		~Response(void);
		std::string	get_data(void) const;
		void	set_data(std::string data);
		Server & parse_virtual_server(Request & request, std::vector<Server> & servers);
		std::string parse_custom_error_pages(std::string error, std::map<std::string, std::string> & error_pages);
		void 	parse_error_pages(std::string error, std::string description, Server & virtual_server);
		void	parse_status_line(std::string status, std::string description);
		Location *	parse_location(Request & request, Server & virtual_server);
		std::string urlDecode(const std::string& input);
		std::string	parse_resource_path(Request & request, Location & location);
		void 	parse_resource(std::string path);
		std::string parse_autoindex_css();
		void 	parse_autoindex(std::string path, std::string route, Server & virtual_server);
		void 	handle_DELETE(std::string path, Server & virtual_server);
		void 	handle_return(std::string return_route);
		void	parse_GET_method(Request & request, Server & virtual_server);
		void	parse_POST_method(Request & request, Server & virtual_server);
		void	parse_DELETE_method(Request & request, Server & virtual_server);
		void	parse_response_data(Request & request, std::vector<Server> & servers);
	private:
		std::string	_data;
};

#endif