/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yetay <yetay@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 16:27:58 by yetay             #+#    #+#             */
/*   Updated: 2023/11/22 16:53:56 by yetay            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "webserv.hpp"

class	Request
{
	public:
		Request(void);
		Request(Request const &cls);
		~Request(void);

		Request const	&operator=(Request const &cls);

		std::string	get_data(void) const;
		void		set_data(std::string &s);

	private:
		std::string	_data;
};

#endif
