/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melee <melee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 16:28:40 by yetay             #+#    #+#             */
/*   Updated: 2023/11/30 09:09:13 by melee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

/* Default constructor */
Request::Request(void) : _data("")
{
	return ;
}

/* Copy constructor */
Request::Request(Request const &cls) : _data("")
{
	*this = cls;
	return ;
}

/* Destructor */
Request::~Request(void)
{
	return ;
}

/* Assignment operator overload */
Request const	&Request::operator=(Request const &cls)
{
	this->_data = cls.get_data();
	return (*this);
}

/* Getter: return value of _data */
std::string	Request::get_data(void) const
{
	return (this->_data);
}

std::string	Request::get_method(void) const 
{
	return (this->_method);
}

std::string	Request::get_route(void) const
{
	return (this->_route);
}

std::string	Request::get_http_protocol(void) const
{
	return (this->_http_protocol);
}

std::string	Request::get_host(void) const
{
	return (this->_host);
}

std::string	Request::get_port(void) const
{
	return (this->_port);
}

std::string	Request::get_content_length(void) const
{
	return (this->_content_length);
}

std::string	Request::get_message_body(void) const
{
	return (this->_message_body);
}

std::map<std::string, std::string> & Request::get_header_fields(void)
{
	return (this->_header_fields);
}

/* Setter: set value of _data */
void	Request::set_data(std::string const &s)
{
	this->_data = s;
	return ;
}

void		Request::parse_request_data()
{
	size_t pos = 0;

	this->parse_request_line(&pos);
	this->parse_header_fields(&pos);
	this->parse_message_body(&pos);
}

void		Request::parse_request_line(size_t *pos)
{
	size_t temp_pos = 0;

	temp_pos = this->_data.find_first_of(" \t", *pos);
	this->_method = this->_data.substr(*pos, temp_pos - *pos);
	*pos = temp_pos + 1;
	temp_pos = this->_data.find_first_of(" \t", *pos);
	this->_route = this->_data.substr(*pos, temp_pos - *pos);
	*pos = temp_pos + 1;
	temp_pos = this->_data.find_first_of(" \n", *pos);
	this->_http_protocol = this->_data.substr(*pos, temp_pos - *pos);
	*pos = temp_pos + 1;
}



void		Request::parse_header_fields(size_t *pos)
{
	size_t endline_delimiter = 0;
	size_t temp_pos = 0;
	
	while (this->_data[*pos] != '\r' && *pos < this->_data.length()) {
		std::pair<std::string, std::string> _pair;
		temp_pos = this->_data.find_first_of(":", *pos);
		_pair.first = this->_data.substr(*pos, temp_pos - *pos);
		*pos = temp_pos + 2;
		if (_pair.first == "Host")
			this->parse_host(pos);
		if (_pair.first == "Content-Length")
			this->parse_content_length(pos);
		temp_pos = this->_data.find_first_of("\n", *pos);
		_pair.second = this->_data.substr(*pos, temp_pos - *pos);
		*pos = temp_pos + 1;
		this->_header_fields.insert(_pair);
	}
}

void		Request::parse_message_body(size_t *pos)
{
	if (this->_content_length != "") {
		*pos += 2;
		size_t temp_pos = 0;
		uint64_t content_length = std::stoll(this->_content_length);
		while (content_length > 0) {
			content_length--;
		}
		this->_message_body = this->_data.substr(*pos, temp_pos - *pos);
	}
}

void Request::parse_host(size_t *pos)
{
	size_t temp_pos = this->_data.find_first_of(":", *pos);
	this->_host = this->_data.substr(*pos, temp_pos - *pos);
	size_t temp_pos2 = temp_pos + 1;
	temp_pos = this->_data.find_first_of("\n", temp_pos2);
	this->_port = this->_data.substr(temp_pos2, temp_pos - temp_pos2 - 1);
}

void Request::parse_content_length(size_t *pos)
{
	size_t temp_pos = this->_data.find_first_of("\n", *pos);
	this->_content_length = this->_data.substr(*pos, temp_pos - *pos);
}

std::ostream & operator<<(std::ostream & out, Request & request) {
	std::cout << "REQUEST ATTRIBUTES" << std::endl;
	std::cout << "==================" << std::endl;
	std::cout << "method: " << request.get_method() << std::endl;
	std::cout << "route: " << request.get_route() << std::endl;
	std::cout << "http_protocol: " << request.get_http_protocol() << std::endl;
	std::cout << "host: " << request.get_host() << std::endl;
	std::cout << "port: " << request.get_port() << std::endl;
	std::cout << "content_length: " << request.get_content_length() << std::endl;
	std::map<std::string, std::string> header_fields = request.get_header_fields();
	std::map<std::string, std::string>::iterator it = header_fields.begin();
	std::map<std::string, std::string>::iterator ite = header_fields.end();
	std::cout << std::endl;
	std::cout << "Header fields" << std::endl;
	std::cout << "-------------" << std::endl;
	for (; it != ite; it++) {
		std::cout << it->first << ":" << it->second << std::endl;
	}
	std::cout << "-------------" << std::endl;
	std::cout << "Message Body" << std::endl;
	std::cout << request.get_message_body() << std::endl;
	std::cout << "==================" << std::endl;
	return out;
}
