/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melee <melee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 17:27:37 by yetay             #+#    #+#             */
/*   Updated: 2023/11/28 16:02:05 by melee            ###   ########.fr       */
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

	i = get_empty();
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

	i = get_fds_index(fd);
	if (i < 0)
		throw CustomException("Poll::update_fd() failure: fd not in Poll::fds");
	fds[i].events = ev;
	fds[i].revents = 0;
	return (0);
}

/* Static function: */
/* Close the current (given index) socket, and remove it from the fds */
void	Poll::close_fd(int i)
{
	close(fds[i].fd);
	memset((void *) (fds + i), 0, sizeof(struct pollfd));
	return ;
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
void	Poll::process(struct addrinfo *res)
{
	for (int i = 0; i < 1024; i++)
	{
		if (fds[i].revents & POLLIN)
		{
			std::cout << "POLLIN: " << fds[i].fd << std::endl;

			if (Connection::is_listen_sockfd(fds[i].fd))
			{
				Connection	*conn = new Connection;
				accept_sock(fds[i].fd, *conn, res);
				Connection::io_conn.push_back(*conn);
			}
			else
			{
				int	conn_index = Connection::get_conn_index(fds[i].fd);
				Connection &conn = Connection::io_conn.at(conn_index);
				recv_data(fds[i].fd, conn);
			}
		}
		else if (fds[i].revents & POLLOUT)
		{
			int			conn_index = Connection::get_conn_index(fds[i].fd);
			Connection	&conn = Connection::io_conn.at(conn_index);

			std::cout << "POLLOUT: " << fds[i].fd << std::endl;

			if (conn.get_request()->get_data().length() == 0)
				continue;

			conn.get_request()->parse_request_data();
			std::cout << *(conn.get_request()) << std::endl;
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
			close_fd(i);
			conn.set_sockfd(0);
			conn.get_request()->set_data("");
			Connection::io_conn.erase(Connection::io_conn.begin()+conn_index);
			continue;
		}
	}
	return ;
}

/* Static getter: return index of first empty struct in fds array */
int	Poll::get_empty(void)
{
	int	i = 0;
	while (fds[i].fd != 0)
		i++;
	return (i);
}

/* Static getter: return index of given fd in fds array  */
int	Poll::get_fds_index(int fd)
{
	for (int i = 0; i < 1024; i++)
	{
		if (fds[i].fd == fd)
			return (i);
	}
	return (-1);
}

/* Static function: accept an incoming connection request */
void	Poll::accept_sock(int fd, Connection &conn, struct addrinfo *res)
{
	conn.set_sockfd(accept(fd, (struct sockaddr*) &res->ai_addr, &res->ai_addrlen));
	if (conn.get_sockfd() < 0)
		throw CustomException("Accept failure: " + std::string(strerror(errno)));
	std::cout << conn.get_sockfd() << std::endl;
	add_fd(conn.get_sockfd(), POLLIN | POLLPRI);
	return ;
}

/* Static function: recv data from an incoming connection request */
void	Poll::recv_data(int fd, Connection &conn)
{
	char	buffer[RECV_BUFFER_SIZE];
	int		recvstat;

	recvstat = recv(fd, buffer, RECV_BUFFER_SIZE - 2, 0);
	if (recvstat < 0)
		throw CustomException("Recv failure: " + std::string(strerror(errno)));
	buffer[recvstat] = 0;
	conn.get_request()->set_data(conn.get_request()->get_data() + std::string(buffer));
	std::cout << conn.get_request()->get_data() << std::endl;
	update_fd(fd, POLLIN | POLLPRI | POLLOUT | POLLWRBAND);
	return ;
}
