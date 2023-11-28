/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melee <melee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 16:27:58 by yetay             #+#    #+#             */
/*   Updated: 2023/11/27 19:11:55 by melee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "webserv.hpp"

class	Request
{
	public:
		Request(void);
		Request(Request const &cls);
		~Request(void);

		Request const	&operator=(Request const &cls);

		std::string	get_data(void) const;
		std::string	get_method(void) const;
		std::string	get_route(void) const;
		std::string	get_http_protocol(void) const;
		std::string	get_host(void) const;
		std::string	get_port(void) const;
		std::string	get_content_length(void) const;
		std::string	get_message_body(void) const;
		std::map<std::string, std::string> & get_header_fields(void);
		void		set_data(std::string const &s);
		void		parse_request_data();
		void		parse_request_line(size_t *pos);
		void		parse_header_fields(size_t *pos);
		void		parse_message_body(size_t *pos);
		void		parse_host(size_t *pos);
		void		parse_content_length(size_t *pos);

	private:
		std::string	_data;
		std::string	_method;
		std::string _route;
		std::string _http_protocol;
		std::string _host;
		std::string _port;
		std::string _content_length;
		std::string _message_body;
		std::map<std::string, std::string> _header_fields;
};

std::ostream & operator<<(std::ostream & out, Request & request);

#endif
