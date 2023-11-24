/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yetay <yetay@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 17:27:37 by yetay             #+#    #+#             */
/*   Updated: 2023/11/24 13:13:46 by yetay            ###   ########.fr       */
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

/* Setter: set value of _fds at index i */
void	Poll::set_fds(int i, int fd, int ev, int rev)
{
	this->_fds[i].fd = fd;
	this->_fds[i].events = ev;
	this->_fds[i].revents = rev;
	return ;
}

/* Polls through the _fds array for ready FDs */
int	Poll::check(void)
{
	return (poll(this->_fds, 1024, 10000));
}

/* Goes through the _fds array and process each ready FD */
void	Poll::process(Connection &conn, struct addrinfo *res)
{
	for (int i = 0; i < 1024; i++)
	{
		if (this->_fds[i].revents & POLLIN)
		{
			std::cout << "POLLIN: " << this->_fds[i].fd << std::endl;

			if (Connection::is_listen_sockfd(this->_fds[i].fd))
			{
				conn.set_sockfd(accept(this->_fds[i].fd, (struct sockaddr*) &res->ai_addr, &res->ai_addrlen));
				if (conn.get_sockfd() < 0)
					throw CustomException("Accept failure: " + std::string(strerror(errno)));
				std::cout << conn.get_sockfd() << std::endl;
				this->set_fds(this->get_empty(), conn.get_sockfd(), POLLIN | POLLPRI | POLLOUT | POLLWRBAND, 0);
			}
			else
			{
				char	buffer[1024];
				int		recvstat;

				recvstat = recv(this->_fds[i].fd, buffer, 1024 - 2, 0);
				if (recvstat < 0)
					throw CustomException("Recv failure: " + std::string(strerror(errno)));
				buffer[recvstat] = 0;
				conn.get_request()->set_data(conn.get_request()->get_data() + std::string(buffer));
				std::cout << conn.get_request()->get_data() << std::endl;
			}
		}
		else if (this->_fds[i].revents & POLLOUT)
		{
			std::cout << "POLLOUT: " << this->_fds[i].fd << std::endl;
			if (conn.get_request()->get_data().length() == 0)
				continue;

			std::string	servMsg;

			servMsg = "HTTP/1.1 200 \r\nContent-Type: text/html\r\n";
			servMsg = servMsg + "\r\n\r\n";
			servMsg = servMsg + "<html><header><title>Go!</title>";
			servMsg = servMsg + "</header><body><p>You did it!</p>";
			servMsg = servMsg + "</body></html>";

			if (send(this->_fds[i].fd, servMsg.c_str(), servMsg.length(), 0) < 0)
				throw CustomException("Send failure: " + std::string(strerror(errno)));
			std::cout << "Response sent." << std::endl;

			// CLOSE CONNECTION AND REMOVE FROM POLL FDS
			close(this->_fds[i].fd);
			memset((void *) (this->_fds + i), 0, sizeof(struct pollfd));
			conn.set_sockfd(0);
			conn.get_request()->set_data("");
			continue;
		}
	}
	return ;
}

/* Getter: return index of first empty struct in _fds */
int	Poll::get_empty(void) const
{
	int	i = 0;
	while (this->_fds[i].fd != 0)
		i++;
	return (i);
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
