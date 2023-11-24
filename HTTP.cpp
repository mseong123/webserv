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
	struct addrinfo	res;

	sockfd = Connection::serv_listen(serv.get_host(), serv.get_port(), &res);

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
		po.process(conn, &res);
	}

};

std::vector<Server> & HTTP::get_server() {
	return this->_server;
}
