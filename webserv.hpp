#ifndef WEBSERV_HPP
#define WEBSERV_HPP
// system headers
# include <iostream>
# include <exception>
# include <fstream>
# include <netdb.h>
# include <netinet/in.h>
# include <sys/poll.h>
# include <sys/socket.h>
# include <unistd.h>
# include <vector>
# include <map>
# include <algorithm>

// class headers
# include "CustomException.hpp"
# include "Config.hpp"
# include "ConfigServer.hpp"
# include "ConfigLocation.hpp"
# include "HTTP.hpp"
# include "Server.hpp"
# include "Location.hpp"
# include "Request.hpp"
# include "Connection.hpp"
# include "Poll.hpp"

# define DEFAULT_CONFIG "default_config"
# define RECV_BUFFER_SIZE 1024

#endif
