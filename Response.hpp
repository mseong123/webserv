#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "webserv.hpp"

class	Response {
	public:
		Response(void);
		Response(Response const &cls);
		~Response(void);
		Response const	&operator=(Response const &cls);
		std::string	get_data(void) const;
		void	parse_response_data(Request & request, std::vector<Server> & servers);
	private:
		std::string	_data;

};

#endif