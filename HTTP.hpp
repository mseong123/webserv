#include "webserv.hpp"

class Server;

class HTTP {
	public:
		HTTP();
		~HTTP();
		std::vector<Server> & get_servers();
		void init(const std::string path);
		void print_servers();
	private:
		std::vector<Server> _servers;
};