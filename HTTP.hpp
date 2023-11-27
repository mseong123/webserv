#include "webserv.hpp"

class Server;
class Connection;

class HTTP {
	public:
		HTTP();
		~HTTP();
		std::vector<Server> & get_servers();
		void init(const std::string path);
	private:
		std::vector<Server> 	_server;
};

std::ostream & operator<<(std::ostream & out, std::vector<Server> & servers);
