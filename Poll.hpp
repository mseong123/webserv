/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yetay <yetay@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 17:26:45 by yetay             #+#    #+#             */
/*   Updated: 2023/11/23 18:06:27 by yetay            ###   ########.fr       */
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
		int					get_polls(void) const;

		void	put_fds(void) const;

	private:
		struct pollfd	_fds[1024];
		int				_polls;
};

std::ostream	&operator<<(std::ostream &out, Poll const &cls);

#endif
