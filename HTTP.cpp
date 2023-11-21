#include "HTTP.hpp"
#include "Config.hpp"

HTTP::HTTP() {
};

HTTP::~HTTP() {
};

void HTTP::init(const std::string path) {
	Config config(path);
	config.parse_file();
};