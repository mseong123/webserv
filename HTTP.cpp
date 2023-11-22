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

};

std::vector<Server> & HTTP::get_server() {
	return this->_server;
}
