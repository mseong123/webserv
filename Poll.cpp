/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yetay <yetay@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 17:27:37 by yetay             #+#    #+#             */
/*   Updated: 2023/11/24 17:42:13 by yetay            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

/* Initialisation for static attributes */
struct pollfd	Poll::fds[1024];

/* Static Getter: */
/* Display all non-zero fds in fds */
void	Poll::put_fds(void)
{
	std::cout << "Poll FDs: ";
	for (int i = 0; i < 1024; i++)
	{
		if (fds[i].fd != 0)
			std::cout << fds[i].fd << " ";
	}
	std::cout << std::endl;
	return ;
}

/* Static function: */
/* Returns true if fds is a member of the array */
bool	Poll::is_fds(int fd)
{
	for (int i = 0; i < 1024; i++)
	{
		if (fds[i].fd == fd)
			return (true);
	}
	return (false);
}

/* Static function: */
/* Adds data to fds */
int	Poll::add_fd(int fd, int ev)
{
	int	i;

	i = Poll::get_empty();
	if (i == 1024)
		return (-1);
	fds[i].fd = fd;
	fds[i].events = ev;
	fds[i].revents = 0;
	return (0);
}

/* Static function: */
/* Update the values of a struct */
int	Poll::update_fd(int fd, int ev)
{
	int	i;

	i = Poll::get_fds_index(fd);
	if (i < 0)
		throw CustomException("Poll::update_fd() failure: fd not in Poll::fds");
	fds[i].events = ev;
	fds[i].revents = 0;
	return (0);
}

/* Static function: */
/* Polls through the fds array for ready FDs */
int	Poll::check(void)
{
	int	polls;

	polls = poll(fds, 1024, -1);
	if (polls < 0)
		throw CustomException("Poll failure: " + std::string(strerror(errno)));
	return (polls);
}

/* Default constructor - DOES NOTHING */
Poll::Poll(void)
{
	return ;
}

/* Copy constructor - DOES NOTHING */
Poll::Poll(Poll const &cls)
{
	(void) cls;
	return ;
}

/* Destructor - DOES NOTHING */
Poll::~Poll(void)
{
	return ;
}

/* Assignment operator overload - DOES NOTHING */
Poll const	&Poll::operator=(Poll const &cls)
{
	(void) cls;
	return (*this);
}

/* Goes through the _fds array and process each ready FD */
void	Poll::process(Connection &conn, struct addrinfo *res)
{
	for (int i = 0; i < 1024; i++)
	{
		if (fds[i].revents & POLLIN)
		{
			std::cout << "POLLIN: " << fds[i].fd << std::endl;

			if (Connection::is_listen_sockfd(fds[i].fd))
			{
				conn.set_sockfd(accept(fds[i].fd, (struct sockaddr*) &res->ai_addr, &res->ai_addrlen));
				if (conn.get_sockfd() < 0)
					throw CustomException("Accept failure: " + std::string(strerror(errno)));
				std::cout << conn.get_sockfd() << std::endl;
				add_fd(conn.get_sockfd(), POLLIN | POLLPRI);
			}
			else
			{
				char	buffer[1024];
				int		recvstat;

				recvstat = recv(fds[i].fd, buffer, 1024 - 2, 0);
				if (recvstat < 0)
					throw CustomException("Recv failure: " + std::string(strerror(errno)));
				buffer[recvstat] = 0;
				conn.get_request()->set_data(conn.get_request()->get_data() + std::string(buffer));
				std::cout << conn.get_request()->get_data() << std::endl;
				update_fd(fds[i].fd, POLLIN | POLLPRI | POLLOUT | POLLWRBAND);
			}
		}
		else if (fds[i].revents & POLLOUT)
		{
			std::cout << "POLLOUT: " << fds[i].fd << std::endl;
			if (conn.get_request()->get_data().length() == 0)
				continue;

			std::string	servMsg;

			servMsg = "HTTP/1.1 200 \r\nContent-Type: text/html\r\n";
			servMsg = servMsg + "\r\n\r\n";
			servMsg = servMsg + "<html><header><title>Go!</title>";
			servMsg = servMsg + "</header><body><p>You did it!</p>";
			servMsg = servMsg + "</body></html>";

			if (send(fds[i].fd, servMsg.c_str(), servMsg.length(), 0) < 0)
				throw CustomException("Send failure: " + std::string(strerror(errno)));
			std::cout << "Response sent." << std::endl;

			// CLOSE CONNECTION AND REMOVE FROM POLL FDS
			close(fds[i].fd);
			memset((void *) (fds + i), 0, sizeof(struct pollfd));
			conn.set_sockfd(0);
			conn.get_request()->set_data("");
			continue;
		}
	}
	return ;
}

/* Getter: return index of first empty struct in fds array */
int	Poll::get_empty(void)
{
	int	i = 0;
	while (fds[i].fd != 0)
		i++;
	return (i);
}

/* Getter: return index of given fd in fds array  */
int	Poll::get_fds_index(int fd)
{
	for (int i = 0; i < 1024; i++)
	{
		if (fds[i].fd == fd)
			return (i);
	}
	return (-1);
}
