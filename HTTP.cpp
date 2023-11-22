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

	int					sockfd;
	int					opt;
	struct sockaddr_in	addr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		throw CustomException("Socket failure: " + std::string(strerror(errno)));
	opt = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
		throw CustomException("Setsockopt failure: " + std::string(strerror(errno)));
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(stoi(serv.get_port()));
	addr.sin_addr.s_addr = INADDR_ANY;
	if (bind(sockfd, (struct sockaddr*) &addr, sizeof(addr)) < 0)
		throw CustomException("Bind failure: " + std::string(strerror(errno)));
	if (listen(sockfd, 3) < 0)
		throw CustomException("Listen failure: " + std::string(strerror(errno)));

	std::cout << "Listening on port " << sockfd << "." << std::endl;

	// Add socket to vector of listening-only sockets
	this->_listen.push_back(sockfd);

	struct pollfd	fds[1024];
	int				polls;

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
		for (int i = 0; i < 1024; i++)
		{
			if (fds[i].revents & POLLIN)
			{
				std::cout << "POLLIN: " << fds[i].fd << std::endl;
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
					Connection	conn;
					socklen_t	addrlen;

					addrlen = sizeof(addr);
					conn.set_sockfd(accept(sockfd, (struct sockaddr*) &addr, &addrlen));
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
					for (int i = 0; i < 1024; i++)
						if (fds[i].fd > 0)
							std::cout << fds[i].fd << " ";
					std::cout << std::endl;
				}
			}
			else if (fds[i].revents & POLLOUT)
				std::cout << "POLLOUT: " << fds[i].fd << std::endl;
		}
	}

};

std::vector<Server> & HTTP::get_server() {
	return this->_server;
}
