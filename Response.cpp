/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lewlee <lewlee@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 10:04:47 by lewlee            #+#    #+#             */
/*   Updated: 2023/11/21 13:42:05 by lewlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

// need minimum GET POST DELETE

// THIS IS FOR GET
// -------------------------------------------------------------------------------------------------------------------
// need to check if need CGI
// if path cient requested has no ending slash then must treat as file else treat it as directory
// if is directory
// 		autoindex on, no index.html:
// 		~ NGINX will generate a directory listing HTML page and return it to the client.
//		autoindex on, index.html exists:
//		~ NGINX will serve the existing index.html file.
//		autoindex off, no index.html:
//		~ NGINX may return a 403 Forbidden error or a different error, depending on the server configuration.
//  	autoindex off, index.html exists:
//  	~ NGINX will serve the existing index.html file.
// else treating it as file
//  	if file found, serve it back
//  	else return 404 page
// -------------------------------------------------------------------------------------------------------------------
// what ouutside variable it needs?
// -------------------------------------------------------------------------------------------------------------------
// sockfd
// autoindex bool
// root dir
// path client requested
// is there a CGI needed?
// cgi name if needed
// -------------------------------------------------------------------------------------------------------------------


// THIS IS FOR POST DELETE
// -------------------------------------------------------------------------------------------------------------------
// HAVE NO FUCKING CLUE WHAT POST DOES OUTSIDE OF DATABASES AND CGI
// HAVE NO FUCKING CLUE WHAT DELETE DOES OUTSIDE OF DATABASES AND CGI
// -------------------------------------------------------------------------------------------------------------------

// if the requested URL has this '\' instead of '/' then it must correct it to '/'

void Response::GET(Request request) {
	
	// WORRY ABOUT CGI LATER
	// -------------------------------------------------------------
	// bool isCGI = false;
	// if () {
	// 	// check for CGI then run and get CGI response
	// }
	
	
	// check if it exist
	if (request.path[request.path.length() - 1] == '/') {
		// check for autoindexing
	}
	if (/* found the index.html */) {
		// send the requested file	
	}
	// return 404 not found page
}

void Response::POST() {
	std::cout << "no clue what dis do" << std::endl;
}

void Response::DELETE() {
	std::cout << "no clue what dis do" << std::endl;
}

std::string	indexHtmlGen() {
	
}