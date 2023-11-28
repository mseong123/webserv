/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melee <melee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 17:26:45 by yetay             #+#    #+#             */
/*   Updated: 2023/11/28 18:53:31 by melee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POLL_HPP
# define POLL_HPP

# include "webserv.hpp"

class	Poll
{
	public:
		static std::vector<struct pollfd>	fds;

		static void	add_fd(int fd, int ev);
		static int	check(void);
		static void	process(std::vector < std::pair<int, struct addrinfo> > &, std::vector<Server> & servers);

		~Poll(void);

	private:
		static bool	is_fds(int fd);
		static void	update_fd(int fd, int ev);
		static void	accept_sock(int fd, Connection &conn, struct addrinfo *res);
		static void	recv_data(int fd, Connection &conn);
		static void	close_fd(int i);

		Poll(void);
		Poll(Poll const &cls);

		Poll const	&operator=(Poll const &cls);
};

std::ostream	&operator<<(std::ostream &out, std::vector<struct pollfd> &v);
#endif
