#include "webserv.hpp"

class Server;

class HTTP {
	public:
		HTTP();
		~HTTP();
		void init(const std::string path);
		std::vector<Server> & get_servers();
	private:
		std::vector<Server> _servers;
		

};