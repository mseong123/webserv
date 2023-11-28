/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melee <melee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 18:37:37 by yetay             #+#    #+#             */
/*   Updated: 2023/11/28 18:49:32 by melee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_HPP
# define CONNECTION_HPP

# include "webserv.hpp"

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
