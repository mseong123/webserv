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
	Connection::listen_sockfds.push_back(sockfd);

	Poll			po;
	int				polls;
	Connection		conn;
	socklen_t		addrlen;

	po.set_fds(0, sockfd, POLLIN | POLLPRI, 0);
	while (true)
	{
		polls = po.check();
		if (polls < 0)
			throw CustomException("Poll failure: " + std::string(strerror(errno)));
		if (polls == 0)
			continue;

		// THIS PART JUST PRINTS OUT THE FDS IN THE POLL FDS ARRAY
		std::cout << po << std::endl;

		// CHECKS THROUGH ALL THE FDS IN THE POLL FDS ARRAY FOR REVENTS
		po.process(conn, res);
	}

};

std::vector<Server> & HTTP::get_server() {
	return this->_server;
}
