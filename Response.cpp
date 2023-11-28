#include "webserv.hpp"

/* Default constructor */
Response::Response(void) : _data("")
{
	return ;
}

/* Copy constructor */
Response::Response(Response const &cls) : _data("")
{
	*this = cls;
	return ;
}

/* Destructor */
Response::~Response(void)
{
	return ;
}

/* Assignment operator overload */
Response const	&Response::operator=(Response const &cls)
{
	this->_data = cls.get_data();
	return (*this);
}

/* Getter: return value of _data */
std::string	Response::get_data(void) const
{
	return (this->_data);
}

void	Response::parse_response_data(Request & request, std::vector<Server> & servers)
{
	std::cout << request << std::endl;
	std::cout << servers << std::endl;

}