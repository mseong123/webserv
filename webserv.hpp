#ifndef WEBSERV_HPP
#define WEBSERV_HPP
// system headers
# include <iostream>
# include <exception>
# include <fstream>
# include <netinet/in.h>
# include <sys/socket.h>
# include <unistd.h>
# include <vector>

// class headers
# include "CustomException.hpp"
# include "Config.hpp"
# include "HTTP.hpp"
# include "Server.hpp"
# include "Location.hpp"
# include "Connection.hpp"

# define DEFAULT_CONFIG "default_config.txt"

#endif
