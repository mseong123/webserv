/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yetay <yetay@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 18:37:37 by yetay             #+#    #+#             */
/*   Updated: 2023/11/27 12:12:03 by yetay            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_HPP
# define CONNECTION_HPP

# include "webserv.hpp"

class	Response {};

class	Connection
{
	public:
		static std::vector<int>	listen_sockfds;
		static std::vector<Connection>	io_conn;

		static bool	is_listen_sockfd(int fd);
		static int	get_conn_index(int sockfd);
		static int	serv_listen(std::string host, std::string port, struct addrinfo *res);

		Connection(void);
		Connection(Connection const &cls);
		~Connection(void);

		Connection const	&operator=(Connection const &cls);

		int			get_sockfd(void) const;
		Request		*get_request(void) const;
		Response	*get_response(void) const;
		void		set_sockfd(int sockfd);
		void		set_request(Request *request);
		void		set_response(Response *response);

	private:
		int			_sockfd;
		Request		*_request;
		Response	*_response;
};

std::ostream	&operator<<(std::ostream &out, std::vector<Connection> &conn);

#endif
