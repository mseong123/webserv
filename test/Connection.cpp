/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yetay <yetay@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 18:53:31 by yetay             #+#    #+#             */
/*   Updated: 2023/11/21 19:04:51 by yetay            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int	main(void)
{
	Connection	conn;

	std::cout << conn.get_sockfd() << std::endl;
	std::cout << conn.get_request() << std::endl;
	std::cout << conn.get_response() << std::endl;
	return (0);
}
