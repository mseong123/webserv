/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Respose.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lewlee <lewlee@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 09:21:59 by lewlee            #+#    #+#             */
/*   Updated: 2023/11/21 09:44:41 by lewlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONES_CLASS_HPP
# define RESPONES_CLASS_HPP

# include <string>

class Response {
	public:
		void GET();
		void POST();
		void DELETE();
	private:
		std::string _response;
};

#endif // RESPONSE_CLASS_HPP