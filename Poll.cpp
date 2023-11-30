/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melee <melee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 17:27:37 by yetay             #+#    #+#             */
/*   Updated: 2023/11/30 19:13:57 by yetay            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include <dirent.h>
#include <sys/stat.h>
#include <ctime>

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
//			conn.get_response().parse_response_data(conn.get_request(), servers);
			(void) servers;
			//std::cout << conn.get_request() << std::endl;
			std::string	servMsg;
			std::string	target;
			std::string	route;

			route = conn.get_request().get_route();
			std::cout << std::endl;
			std::cout << "ROUTE: " << route << std::endl;
			std::cout << std::endl;
			target = "/static-site/";
			if (route.compare(0, target.length(), target) != 0)
			{
				servMsg = "HTTP/1.1 200 \r\nContent-Type: text/html\r\n";
				servMsg = servMsg + "\r\n\r\n";
				servMsg = servMsg + "<html><header><title>Go!</title>";
				servMsg = servMsg + "</header><body><p>You did it!</p>";
				servMsg = servMsg + "</body></html>";
			}
			else
			{
				std::string	filename;
				std::string	line_to_serve;

				filename = route.substr(1);
				if (route.back() == '/')
				{ // SERVE DIRECTORY LISTING
					DIR				*dir = opendir(filename.c_str());
					struct dirent	*dp;
					std::ifstream	style("default.css");
					std::string		line;

					servMsg = "HTTP/1.1 404 \r\nContent-Type: text/html\r\n";
					servMsg = servMsg + "\r\n\r\n";
					servMsg = servMsg + "<html><header><title>Error: 404";
					servMsg = servMsg + "</title></header><body><h1>404</h1>";
					servMsg = servMsg + "<p>File not found.</p>";
					servMsg = servMsg + "</body></html>";
					if (dir != NULL)
					{
						std::string	file_content;

						servMsg = "HTTP/1.1 200 \r\n";
						servMsg = servMsg + "Content-Type: text/html\r\n";

						file_content = "<html><header><title>";
						file_content += filename;
						file_content += "</title><style>";
						while (getline(style, line))
							file_content += line;
						file_content += "</style></header>";
						file_content += "<body><h1>Index of ";
						file_content += filename;
						file_content += "</h1><div class='dls'>";
						file_content += "<div class='row'>";
						file_content += "<div class='fn'>Filename</div>";
						file_content += "<div class='tm'>Last Modified</div>";
						file_content += "<div class='size'>File Size</div>";
						file_content += "</div><hr />";

						dp = readdir(dir);
						while (dp != NULL)
						{
							if (std::string(dp->d_name).compare(".") != 0)
							{
								std::string	fn = filename + dp->d_name;
								DIR			*tmp = opendir(fn.c_str());
								struct stat	fs;
								struct tm	*tm;
								char		dt[1024];

								memset(&dt, 0, sizeof(dt));
								file_content += "<div class='row'>";

								file_content += "<div class='fn'><a href='";
								file_content += dp->d_name;
								if (tmp != NULL)
									file_content += "/";
								file_content += "'>";
								file_content += dp->d_name;
								if (tmp != NULL)
									file_content += "/";
								file_content += "</a></div>";

								stat(fn.c_str(), &fs);
								tm = localtime(&fs.st_mtime);
								strftime(dt, sizeof(dt), "%x %r", tm);
								file_content += "<div class='tm'>";
								file_content += dt;
								file_content += "</div>";

								file_content += "<div class='size'>";
								if (tmp != NULL)
									file_content += "-";
								else if (stat(fn.c_str(), &fs) == 0)
								{
									std::string	unit;
									size_t		size = fs.st_size;

									if (size > 1000000000000)
									{
										size /= 1000000000;
										unit = "g";
									}
									else if (size > 1000000000)
									{
										size /= 1000000;
										unit = "m";
									}
									else if (size > 1000000)
									{
										size /= 1000;
										unit = "k";
									}
									else
										unit = "";
									file_content += std::to_string(size);
									file_content += unit;
								}
								file_content += "</div>";

								file_content += "</div>";

								if (tmp != NULL)
									closedir(tmp);
							}
							dp = readdir(dir);
						}
						file_content += "</div></body></html>";

						servMsg += "Content-Length: ";
						servMsg += std::to_string(file_content.length());
						servMsg += "\r\n\r\n";
						servMsg += file_content;
					}
					closedir(dir);
				}
				else
				{
					std::ifstream	file_to_serve(filename);

					servMsg = "HTTP/1.1 ";
					if (file_to_serve.is_open() == false)
					{
						servMsg = "HTTP/1.1 404 \r\nContent-Type: text/html\r\n";
						servMsg = servMsg + "\r\n\r\n";
						servMsg = servMsg + "<html><header><title>Error: 404";
						servMsg = servMsg + "</title></header><body><h1>404</h1>";
						servMsg = servMsg + "<p>File not found.</p>";
						servMsg = servMsg + "</body></html>";
					}
					else
					{
						std::string	file_content;
						std::string	file_type;

						file_content = "";
						while (getline(file_to_serve, line_to_serve))
							file_content = file_content + "\n" + line_to_serve;

						file_to_serve.close();

						file_type = filename.substr(filename.find_last_of(".") + 1);
						std::cout << file_type << std::endl;

						servMsg = servMsg + "200 \r\n";
						servMsg = servMsg + "Content-Type: ";
						if (file_type.compare("html") == 0)
							servMsg = servMsg + "text/html";
						else if (file_type.compare("css") == 0)
							servMsg = servMsg + "text/css";
						else if (file_type.compare("jpg") == 0)
							servMsg = servMsg + "image/jpeg";
						else if (file_type.compare("png") == 0)
							servMsg = servMsg + "image/png";
						else if (file_type.compare("pdf") == 0)
							servMsg = servMsg + "application/pdf";
						else
							servMsg = servMsg + "text/plain";

						servMsg = servMsg + "\r\n";
						servMsg = servMsg + "Content-Length: ";
						servMsg = servMsg + std::to_string(file_content.length());
						servMsg = servMsg + "\r\n";
						servMsg = servMsg + file_content;
						if (file_type.compare("csv") == 0 || file_type.compare("txt") == 0)
							servMsg = servMsg + "\n";
					}
				}
			}

			servMsg = servMsg + "\r\n";

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
