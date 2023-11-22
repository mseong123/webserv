#include "webserv.hpp"

class Server;
class Connection;

class HTTP {
	public:
		HTTP();
		~HTTP();
		void init(const std::string path);
		std::vector<Server> & get_server();
	private:
		std::vector<Server> 	_server;
		std::vector<int>		_listen;
		std::vector<Connection>	_conn;	

};
