/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yetay <yetay@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 17:26:45 by yetay             #+#    #+#             */
/*   Updated: 2023/11/23 21:47:08 by yetay            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POLL_HPP
# define POLL_HPP

# include "webserv.hpp"

class	Poll
{
	public:
		Poll(void);
		Poll(Poll const &cls);
		~Poll(void);

		Poll const	&operator=(Poll const &cls);

		struct pollfd const	&get_fds(int i) const;

		void	set_fds(int i, int fd, int ev, int rev);

		int		check(void);
		void	process(Connection &conn, std::vector<int> listens, struct addrinfo *res);

	private:
		struct pollfd	_fds[1024];

		int	get_empty(void) const;
};

std::ostream	&operator<<(std::ostream &out, Poll const &cls);

#endif
