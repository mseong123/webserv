#include "webserv.hpp"

HTTP::HTTP() {
};

HTTP::~HTTP() {
};

void HTTP::init(const std::string path) {
	//Config config(path);
	//config.parse_file(this->get_server());

	(void) path;

	Server	serv;
	serv.set_host("127.0.0.1");
	serv.set_port("8880");
	serv.set_server_names("localhost");

	std::cout << serv.get_host() << std::endl;
	std::cout << serv.get_port() << std::endl;
	std::cout << serv.get_server_names() << std::endl;

	int				sockfd;
	int				opt;
	struct addrinfo	hint, *res;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		throw CustomException("Socket failure: " + std::string(strerror(errno)));
	opt = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
		throw CustomException("Setsockopt failure: " + std::string(strerror(errno)));
	memset(&hint, 0, sizeof(hint));
	hint.ai_family = AF_INET;
	hint.ai_protocol = 0;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_flags = AI_NUMERICSERV;
	if (getaddrinfo(serv.get_host().c_str(), serv.get_port().c_str(), &hint, &res) < 0)
		throw CustomException("Getaddrinfo failure: " + std::string(gai_strerror(errno)));
	if (bind(sockfd, res->ai_addr, res->ai_addrlen) < 0)
		throw CustomException("Bind failure: " + std::string(strerror(errno)));
	if (listen(sockfd, 3) < 0)
		throw CustomException("Listen failure: " + std::string(strerror(errno)));

	std::cout << "Listening on port " << sockfd << "." << std::endl;

	// Add socket to vector of listening-only sockets
	this->_listen.push_back(sockfd);

	struct pollfd	fds[1024];
	int				polls;
	Connection		conn;
	socklen_t		addrlen;

	memset(fds, 0, sizeof(fds));
	fds[0].fd = sockfd;
	fds[0].events = POLLIN | POLLPRI;
	while (true)
	{
		polls = poll(fds, 1024, 10000);
		if (polls < 0)
			throw CustomException("Poll failure: " + std::string(strerror(errno)));
		if (polls == 0)
			continue;

		// THIS PART JUST PRINTS OUT THE FDS IN THE POLL FDS ARRAY
		std::cout << "Polls: ";
		for (int i = 0; i < 1024; i++)
			if (fds[i].fd > 0)
				std::cout << fds[i].fd << " ";
		std::cout << std::endl;

		// CHECKS THROUGH ALL THE FDS IN THE POLL FDS ARRAY FOR REVENTS
		for (int i = 0; i < 1024; i++)
		{
			if (fds[i].revents & POLLIN)
			{
				std::cout << "POLLIN: " << fds[i].fd << std::endl;

				// THIS PART CHECKS IF THE SOCKET IS A LISTENING-ONLY SOCKET
				// SHOULD BE A UTILITY FUNCTION
				// IF TRUE, CREATE NEW CONNECTION
				bool	connect = false;
				for (std::vector<int>::iterator it = _listen.begin(); it != _listen.end(); it++)
				{
					if (*it == fds[i].fd)
					{
						connect = true;
						break;
					}
				}

				if (connect == true)
				{
					conn.set_sockfd(accept(sockfd, (struct sockaddr*) &res->ai_addr, &res->ai_addrlen));
					if (conn.get_sockfd() < 0)
						throw CustomException("Accept failure: " + std::string(strerror(errno)));
					std::cout << conn.get_sockfd() << std::endl;
					for (int i = 0; i < 1024; i++)
					{
						if (fds[i].fd != 0)
						{
							continue;
						}
						else
						{
							fds[i].fd = conn.get_sockfd();
							fds[i].events = POLLIN | POLLPRI | POLLOUT | POLLWRBAND;
							break;
						}
					}
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
	}

};

std::vector<Server> & HTTP::get_server() {
	return this->_server;
}
