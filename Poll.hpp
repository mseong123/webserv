/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yetay <yetay@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 17:26:45 by yetay             #+#    #+#             */
/*   Updated: 2023/11/24 18:01:23 by yetay            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POLL_HPP
# define POLL_HPP

# include "webserv.hpp"

class	Poll
{
	public:
		static struct pollfd	fds[1024];

		static void	put_fds(void);
		static bool	is_fds(int fd);
		static int	add_fd(int fd, int ev);
		static int	update_fd(int fd, int ev);
		static void	close_fd(int i);

		static int	check(void);
		static void	process(Connection &conn, struct addrinfo *res);

		Poll(void);
		Poll(Poll const &cls);
		~Poll(void);

		Poll const	&operator=(Poll const &cls);

	private:
		static int	get_empty(void);
		static int	get_fds_index(int fd);
		static void	accept_sock(int fd, Connection &conn, struct addrinfo *res);
		static void	recv_data(int fd, Connection &conn);
};

#endif
