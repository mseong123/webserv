#include "webserv.hpp"

class Server;

class HTTP {
	public:
		HTTP();
		~HTTP();
		std::vector<Server> & get_servers();
		void init(const std::string path);
	private:
		std::vector<Server> _servers;
};

std::ostream & operator<<(std::ostream & out, std::vector<Server> & servers);