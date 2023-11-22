#include "webserv.hpp"

class Server;

class HTTP {
	public:
		HTTP();
		~HTTP();
		void init(const std::string path);
		std::vector<Server> & get_server();
	private:
		std::vector<Server> _server;
		

};