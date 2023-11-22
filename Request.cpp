/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yetay <yetay@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 16:28:40 by yetay             #+#    #+#             */
/*   Updated: 2023/11/22 16:52:35 by yetay            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

/* Default constructor */
Request::Request(void) : _data("")
{
	return ;
}

/* Copy constructor */
Request::Request(Request const &cls) : _data("")
{
	*this = cls;
	return ;
}

/* Destructor */
Request::~Request(void)
{
	return ;
}

/* Assignment operator overload */
Request const	&Request::operator=(Request const &cls)
{
	this->_data = cls.get_data();
	return (*this);
}

/* Getter: return value of _data */
std::string	Request::get_data(void) const
{
	return (this->_data);
}

/* Setter: set value of _data */
void	Request::set_data(std::string &s)
{
	this->_data = s;
	return ;
}
