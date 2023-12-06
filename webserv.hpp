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
# include <stdio.h>
# include <dirent.h>
# include <sys/stat.h>
# include <ctime>
# include <stdio.h>
# include <fcntl.h>
#include <sys/ioctl.h>

// class headers
# include "CustomException.hpp"
# include "Config.hpp"
# include "ConfigServer.hpp"
# include "ConfigLocation.hpp"
# include "HTTP.hpp"
# include "Server.hpp"
# include "Location.hpp"
# include "Request.hpp"
# include "Response.hpp"
# include "Connection.hpp"
# include "Poll.hpp"
# include "CgiHandler.hpp"



# define DEFAULT_CONFIG "default_config"
# define RECV_BUFFER_SIZE 250
# define HTTP_PROTOCOL "HTTP/1.1"
# define LISTEN_BACKLOG 20000

extern int errno;

#endif
