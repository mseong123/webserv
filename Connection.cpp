/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melee <melee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 18:42:10 by yetay             #+#    #+#             */
/*   Updated: 2023/12/03 19:26:04 by melee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

/* Static member attributes initiations */
std::vector<int>		Connection::listen_sockfds;
std::vector<Connection>	Connection::io_conn;

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

/* Static getter: */
/* returns the index of io_conn containing the instance of a Connection with */
/* the given sockfd */
int	Connection::get_conn_index(int sockfd)
{
	for (unsigned long i = 0; i < io_conn.size(); i++)
	{
		if (io_conn.at(i)._sockfd == sockfd)
			return (i);
	}
	throw CustomException("Unknown connection!");
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
	if (listen(sockfd, LISTEN_BACKLOG) < 0)
		throw CustomException("Listen failure: " + std::string(strerror(errno)));

	std::cout << "Listening on port " << sockfd << "." << std::endl;

	// Add socket to vector of listening-only sockets
	listen_sockfds.push_back(sockfd);

	return (sockfd);
}

/* Default constructor */
Connection::Connection(void) : _sockfd(0)
{
	memset(&_request, 0, sizeof(_request));
	memset(&_response, 0, sizeof(_response));
	return ;
}

///* Copy constructor */
//Connection::Connection(Connection &cls)
//{
//	*this = cls;
//	return ;
//}

/* Destructor */
Connection::~Connection(void)
{
	return ;
}

///* Assignment operator overload */
//Connection	&Connection::operator=(Connection &cls)
//{
//	if (this != &cls)
//	{
//		this->_sockfd = cls.get_sockfd();
//		this->_request = cls.get_request();
//		this->_response = cls.get_response();
//	}
//	return (*this);
//}

/* Getter: return value of _sockfd */
int	Connection::get_sockfd(void) const
{
	return (this->_sockfd);
}

/* Getter: return value of _request */
Request	&Connection::get_request(void)
{
	return (this->_request);
}

/* Getter: return value of _response */
Response	&Connection::get_response(void)
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
void	Connection::set_request(Request &request)
{
	this->_request = request;
	return ;
}

/* Getter: set value of _response */
void	Connection::set_response(Response &response)
{
	this->_response = response;
	return ;
}

/* Insertion operator overload: vector of Connection objects */
std::ostream	&operator<<(std::ostream &out, std::vector<Connection> &conn)
{
	for (unsigned long i = 0; i < conn.size(); i++)
	{
		if (i == 0)
			out << "Socket FDs: ";
		else
			out << " ";
		out << conn.at(i).get_sockfd();
	}
	return (out);
}
