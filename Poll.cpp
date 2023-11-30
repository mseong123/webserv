/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melee <melee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 17:27:37 by yetay             #+#    #+#             */
/*   Updated: 2023/11/29 19:31:29 by yetay            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

/* Initialisation for static attributes */
std::vector<struct pollfd>	Poll::fds;

/* Static function: */
/* Adds data to fds */
void	Poll::add_fd(int fd, int ev)
{
	struct pollfd	newfd;

	memset(&newfd, 0, sizeof(newfd));
	newfd.fd = fd;
	newfd.events = ev;
	fds.push_back(newfd);
	return ;
}

/* Static function: */
/* Polls through the fds array for ready FDs */
int	Poll::check(void)
{
	int	polls;

	polls = poll(fds.data(), fds.size(), -1);
	if (polls < 0)
		throw CustomException("Poll failure: " + std::string(strerror(errno)));
	return (polls);
}

/* Goes through fds and process each ready FD */
void	Poll::process(std::vector< std::pair<int, struct addrinfo> > &socks, std::vector<Server> & servers)
{
	for (size_t i = 0; i < fds.size(); i++)
	{
		if (fds.at(i).revents & POLLIN)
		{
			std::cout << "POLLIN: " << fds.at(i).fd << std::endl;

			if (Connection::is_listen_sockfd(fds.at(i).fd))
			{
				struct addrinfo	*res;
				Connection		*conn = new Connection;

				res = NULL;
				for (size_t i = 0; i < socks.size(); i++)
				{
					if (socks.at(i).first == fds.at(i).fd)
					{
						res = &(socks.at(i).second);
						break ;
					}
				}
				if (res == NULL)
					throw CustomException("Accept failure: unknown listening FD");
				accept_sock(fds.at(i).fd, *conn, res);
				Connection::io_conn.push_back(*conn);
			}
			else
			{
				int			conn_ind = Connection::get_conn_index(fds.at(i).fd);
				Connection	&conn = Connection::io_conn.at(conn_ind);
				recv_data(fds.at(i).fd, conn);
			}
		}
		else if (fds.at(i).revents & POLLOUT)
		{
			int			conn_ind = Connection::get_conn_index(fds.at(i).fd);
			Connection	&conn = Connection::io_conn.at(conn_ind);

			std::cout << "POLLOUT: " << fds.at(i).fd << std::endl;

			if (conn.get_request().get_data().length() == 0)
				continue;

			conn.get_request().parse_request_data();
			conn.get_response().parse_response_data(conn.get_request(), servers);
			//std::cout << conn.get_request() << std::endl;
			std::string	servMsg;

			servMsg = "HTTP/1.1 200 \r\nContent-Type: text/html\r\n";
			servMsg = servMsg + "\r\n\r\n";
			servMsg = servMsg + "<html><header><title>Go!</title>";
			servMsg = servMsg + "</header><body><p>You did it!</p>";
			servMsg = servMsg + "</body></html>";

			if (send(fds.at(i).fd, servMsg.c_str(), servMsg.length(), 0) < 0)
				throw CustomException("Send failure: " + std::string(strerror(errno)));
			std::cout << "Response sent." << std::endl;

			close_fd(i);
			continue;
		}
	}
	return ;
}

/* Destructor - DOES NOTHING */
Poll::~Poll(void)
{
	return ;
}

/* Static function: */
/* Returns true if fds is a member of the array */
bool	Poll::is_fds(int fd)
{
	for (size_t i = 0; i < fds.size(); i++)
	{
		if (fds.at(i).fd == fd)
			return (true);
	}
	return (false);
}

/* Static function: */
/* Update the values of a struct */
void	Poll::update_fd(int fd, int ev)
{
	for (size_t i = 0; i < fds.size(); i++)
	{
		if (fds.at(i).fd == fd)
		{
			fds.at(i).events = ev;
			fds.at(i).revents = 0;
			return ;
		}
	}
	throw CustomException("Update Poll FD failure: Unknown Socket");
}

/* Static function: */
/* Close the current (given index) socket, and remove it from the fds */
void	Poll::close_fd(int i)
{
	int			conn_ind = Connection::get_conn_index(fds.at(i).fd);
	Connection	&conn = Connection::io_conn.at(conn_ind);

	conn.set_sockfd(0);
	conn.get_request().set_data("");
	Connection::io_conn.erase(Connection::io_conn.begin() + conn_ind);
	close(fds.at(i).fd);
	fds.erase(fds.begin() + i);
	return ;
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
	conn.get_request().set_data(conn.get_request().get_data() + std::string(buffer));
	std::cout << conn.get_request().get_data() << std::endl;
	update_fd(fd, POLLIN | POLLPRI | POLLOUT | POLLWRBAND);
	return ;
}

/* Default constructor - DOES NOTHING */
/* IS PRIVATE as the Poll class is not supposed to be instantiated */
Poll::Poll(void)
{
	return ;
}

/* Copy constructor - DOES NOTHING */
/* IS PRIVATE as the Poll class is not supposed to be instantiated */
Poll::Poll(Poll const &cls)
{
	(void) cls;
	return ;
}

/* Assignment operator overload - DOES NOTHING */
/* IS PRIVATE as the Poll class is not supposed to be instantiated */
Poll const	&Poll::operator=(Poll const &cls)
{
	(void) cls;
	return (*this);
}

/* Insertion operator overload (Vector of pollfd structs) */
/* Displays all fd value in each struct */
std::ostream	&operator<<(std::ostream &out, std::vector<struct pollfd> &v)
{
	out << "Poll FDs: ";
	for (size_t i = 0; i < v.size(); i++)
		out << v.at(i).fd << " ";
	out << std::endl;
	return (out);
}
