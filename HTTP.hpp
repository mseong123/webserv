#include "webserv.hpp"
#include "CustomException.hpp"

class HTTP {
	public:
		HTTP();
		~HTTP();
		void init(const std::string path);

};