/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yetay <yetay@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 18:42:10 by yetay             #+#    #+#             */
/*   Updated: 2023/11/27 10:21:40 by yetay            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

/* Static member attributes initiations */
std::vector<int>	Connection::listen_sockfds;

/* Static getter: */
/* returns true if given sockfd is in the listen-only sockFD vector */
bool	Connection::is_listen_sockfd(int fd)
{
	std::vector<int>::iterator 	it;

	for (it = listen_sockfds.begin(); it != listen_sockfds.end(); it++)
	{
		if (*it == fd)
			return (true);
	}
	return (false);
}

/* Static function: */
/*   Start listening on the given port for the given host */
int	Connection::serv_listen(std::string host, std::string port, struct addrinfo *res)
{
	int				sockfd;
	int				opt;
	struct addrinfo	hint;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		throw CustomException("Socket failure: " + std::string(strerror(errno)));
	opt = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
		throw CustomException("Setsockopt failure: " + std::string(strerror(errno)));
	memset(&hint, 0, sizeof(hint));
	hint.ai_family = AF_INET;
	hint.ai_protocol = 0;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_flags = AI_NUMERICSERV;
	if (getaddrinfo(host.c_str(), port.c_str(), &hint, &res) != 0)
		throw CustomException("Getaddrinfo failure: " + std::string(gai_strerror(errno)));
	if (bind(sockfd, res->ai_addr, res->ai_addrlen) < 0)
		throw CustomException("Bind failure: " + std::string(strerror(errno)));
	if (listen(sockfd, 3) < 0)
		throw CustomException("Listen failure: " + std::string(strerror(errno)));

	std::cout << "Listening on port " << sockfd << "." << std::endl;

	// Add socket to vector of listening-only sockets
	listen_sockfds.push_back(sockfd);

	return (sockfd);
}

/* Default constructor */
Connection::Connection(void) : _sockfd(0), 
								_request(new Request), 
								_response(new Response)
{
	return ;
}

/* Copy constructor */
Connection::Connection(Connection const &cls) : _request(new Request),
												_response(new Response)
{
	*this = cls;
	return ;
}

/* Destructor */
Connection::~Connection(void)
{
	delete this->_request;
	delete this->_response;
	// remove sockfd from poll fds
	close(this->_sockfd);
	return ;
}

/* Assignment operator overload */
Connection const	&Connection::operator=(Connection const &cls)
{
	this->_sockfd = cls.get_sockfd();
	this->_request = cls.get_request();
	this->_response = cls.get_response();
	return (*this);
}

/* Getter: return value of _sockfd */
int	Connection::get_sockfd(void) const
{
	return (this->_sockfd);
}

/* Getter: return value of _request */
Request	*Connection::get_request(void) const
{
	return (this->_request);
}

/* Getter: return value of _response */
Response	*Connection::get_response(void) const
{
	return (this->_response);
}

/* Getter: set value of _sockfd */
void	Connection::set_sockfd(int sockfd)
{
	this->_sockfd = sockfd;
	return ;
}

/* Getter: set value of _request */
void	Connection::set_request(Request *request)
{
	this->_request = request;
	return ;
}

/* Getter: set value of _response */
void	Connection::set_response(Response *response)
{
	this->_response = response;
	return ;
}
