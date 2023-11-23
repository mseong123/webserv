/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yetay <yetay@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 17:26:45 by yetay             #+#    #+#             */
/*   Updated: 2023/11/23 20:35:41 by yetay            ###   ########.fr       */
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

		void	set_fds(struct pollfd const &pollfd);

		void	put_fds(void) const;

	private:
		struct pollfd	_fds[1024];
};

std::ostream	&operator<<(std::ostream &out, Poll const &cls);

#endif
