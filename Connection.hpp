/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yetay <yetay@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 18:37:37 by yetay             #+#    #+#             */
/*   Updated: 2023/11/24 09:17:07 by yetay            ###   ########.fr       */
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

		static bool	is_listen_sockfd(int fd);

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

#endif
