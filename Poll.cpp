/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yetay <yetay@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 17:27:37 by yetay             #+#    #+#             */
/*   Updated: 2023/11/23 20:37:25 by yetay            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

/* Default constructor */
Poll::Poll(void)
{
	memset(this->_fds, 0, sizeof(this->_fds));
	return ;
}

/* Copy constructor */
Poll::Poll(Poll const &cls)
{
	*this = cls;
	return ;
}

/* Destructor */
Poll::~Poll(void)
{
	return ;
}

/* Assignment operator overload */
Poll const	&Poll::operator=(Poll const &cls)
{
	for (int i = 0; i < 1024; i++)
		this->_fds[i] = cls.get_fds(i);
	return (*this);
}

/* Getter: return value of _fds at index i */
struct pollfd const	&Poll::get_fds(int i) const
{
	return (this->_fds[i]);
}

/* Insertion operator overload (OSTREAM) */
std::ostream	&operator<<(std::ostream &out, Poll const &cls)
{
	out << "Poll FDs: ";
	for (int i = 0; i < 1024; i++)
	{
		if (cls.get_fds(i).fd != 0)
			out << cls.get_fds(i).fd << " ";
	}
	return (out);
}
