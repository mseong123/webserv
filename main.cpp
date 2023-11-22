#include "webserv.hpp"

int main (int argc, char **argv) {
	HTTP http;
	try {
		if (argc == 1)
			http.init(std::string(DEFAULT_CONFIG));
		else if (argc == 2) 
			http.init(std::string(argv[1]));
		else
			throw CustomException("Incorrect number of arguments");
	}
	catch (CustomException & e) {
		std::cout<< e.what() << std::endl;
	}
	catch (std::exception & e) {
		std::cout<< e.what()<< std::endl;
	}
	return (0);
}