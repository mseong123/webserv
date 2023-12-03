#include "webserv.hpp"

/* Default constructor */
Response::Response(void) : _data("")
{
	return ;
}

/* Copy constructor */
Response::Response(Response const &cls) : _data("")
{
	*this = cls;
	return ;
}

/* Destructor */
Response::~Response(void)
{
	return ;
}

/* Assignment operator overload */
Response const	&Response::operator=(Response const &cls)
{
	this->_data = cls.get_data();
	return (*this);
}

/* Getter: return value of _data */
std::string	Response::get_data(void) const
{
	return (this->_data);
}

Server 	& Response::parse_virtual_server(Request & request, std::vector<Server> & servers) {
	std::vector<Server>::iterator it = servers.begin();
	std::vector<Server>::iterator ite = servers.end();
	int found = 0;
	int index = 0;
	for (; it != ite; it++) {
		if (request.get_port() == it->get_port()) {
			found = 1;
			if (request.get_host() == it->get_server_name())
				return *it;
		}
		if (!found)
			index++;
	}
	return servers[index];
}

std::string 	Response::parse_custom_error_pages(std::string error, std::map<std::string, std::string> & error_pages) {
	std::map<std::string, std::string>::iterator it = error_pages.begin();
	std::map<std::string, std::string>::iterator ite = error_pages.end();
	std::string buffer;
	std::string temp_message_body;

	for (; it != ite; it++) {
		if (it->first == error) {
			std::fstream fs(it->second, std::fstream::in);
			if (fs.is_open())
				while(std::getline(fs, buffer, '\n'))
					temp_message_body += buffer;
		}
	}
	return temp_message_body;
}

void 	Response::parse_error_pages(std::string error, std::string description, Server & virtual_server) {
	std::string temp_message_body = this->parse_custom_error_pages(error, virtual_server.get_error_pages());

	if (this->_data == "") {
		if (temp_message_body == "")
			temp_message_body.append("<html><head><title>").append(error).append(" ").append(description).append("</title></head><body><center><h1>")\
			.append(error).append(" ").append(description).append("</h1></center></body></html>");
		this->parse_status_line(error, description);
		this->_data.append("Content-Type: text/html\r\n").append("Content-Length: ").append(std::to_string(temp_message_body.length())).append("\r\n").append("\r\n");
		this->_data.append(temp_message_body);
		this->_data.append("\r\n");
	}
}

void 	Response::parse_status_line(std::string status, std::string description) {
	this->_data = std::string(HTTP_PROTOCOL) + " " + status + " " + description + "\r\n";
}

Location *	Response::parse_location(Request & request, Server & virtual_server) {
	std::vector<Location>::iterator it = virtual_server.get_location().begin();
	std::vector<Location>::iterator ite = virtual_server.get_location().end();
	std::string req = request.get_route();
	std::string req_path = req.substr(0, req.substr(1, req.length()).find_last_of("/") != std::string::npos? req.substr(1, req.length()).find_last_of("/") + 1 : 1);
	int found = 0;
	int index = 0;
	
	for (; it != ite; it++) {
		if (it->get_route() == req_path.substr(0, it->get_route().length())) {
			found = 1;
			if (it->get_route() == req_path)
				return &(*it); 
		}
		if (!found)
			index++;
	}
	if (found)
		return &(virtual_server.get_location()[index]);
	else
		return NULL;
}

std::string	Response::parse_resource_path(Request & request, Location & location) {
	std::string resource_path = ".";

	if (location.get_root() != "")
		resource_path += location.get_root();
	resource_path += request.get_route();
	return resource_path;
}

void Response::parse_resource(std::string path) {
	std::fstream	resource(path);
	if (resource.is_open()) {
		std::string	resource_content;
		std::string	resouce_type;
		std::string line;

		while (getline(resource, line))
			resource_content = resource_content + "\n" + line;

		resource.close();
		resouce_type = path.substr(path.find_last_of(".") + 1);
		this->_data += std::string(HTTP_PROTOCOL) + " 200 OK\r\n";
		this->_data += "Content-Type: ";
		if (resouce_type.compare("html") == 0)
			this->_data += "text/html";
		else if (resouce_type.compare("css") == 0)
			this->_data += "text/css";
		else if (resouce_type.compare("js") == 0)
			this->_data += "text/javascript";
		else if (resouce_type.compare("jpg") == 0)
			this->_data += "image/jpeg";
		else if (resouce_type.compare("png") == 0)
			this->_data += "image/png";
		else if (resouce_type.compare("pdf") == 0)
			this->_data += "application/pdf";
		else
			this->_data += "text/plain";

		this->_data += "\r\n";
		this->_data += "Content-Length: ";
		this->_data += std::to_string(resource_content.length());
		this->_data += "\r\n";
		this->_data += resource_content;
		if (resouce_type.compare("csv") == 0 || resouce_type.compare("txt") == 0)
			this->_data += "\n";
		this->_data += "\r\n";
	}
}

std::string Response::parse_autoindex_css() {
	std::string css;

	css += "body {\n\tmargin: 0 auto;\n\twidth: 75vw;\n\tfont-family: \"Courier New\", fixed;\n\tfont-size: 16pt;\n}\n";
	css += "hi {\n\tmargin: 42px 0 14px;\n}\n";
	css += "div.dls {\n\tmargin-top: 16px;\n\tpadding-top: 16px;\n}\n";
	css += "hr {\n\tmargin: 16 0;\n\tborder-top: 2px solid black;\n}\n";
	css += "div.row {\n\tdisplay: flex;\n\tmargin: 4px 0;\n}\n";
	css += "div.fn {\n\twidth: 40%;\n}\n";
	css += "div.tm {\n\twidth: 35%;\n}\n";
	css += "div.size {\n\twidth: 15%;\n\ttext-align: right;\n}\n";
	return css;
}

void 	Response::parse_autoindex(std::string path, std::string route, Server & virtual_server) {
	DIR				*dir = opendir(path.c_str());
	struct dirent	*dp;
	std::string		line;

	if (dir != NULL)
	{
		std::string	dir_content;
		std::string dirname = route.substr(1);

		this->_data = "HTTP/1.1 200 OK\r\n";
		this->_data = this->_data + "Content-Type: text/html\r\n";

		dir_content = "<html><header><title>";
		dir_content += dirname;
		dir_content += "</title><style>";
		dir_content += parse_autoindex_css();
		dir_content += "</style></header>";
		dir_content += "<body><h1>Index of ";
		dir_content += dirname;
		dir_content += "</h1><div class='dls'>";
		dir_content += "<div class='row'>";
		dir_content += "<div class='fn'>Filename</div>";
		dir_content += "<div class='tm'>Last Modified</div>";
		dir_content += "<div class='size'>File Size</div>";
		dir_content += "</div><hr />";

		dp = readdir(dir);
		while (dp != NULL)
		{
			if (std::string(dp->d_name).compare(".") != 0)
			{
				std::string	fn = dirname + dp->d_name;
				DIR			*tmp = opendir(fn.c_str());
				struct stat	fs;
				struct tm	*tm;
				char		dt[1024];

				memset(&dt, 0, sizeof(dt));
				dir_content += "<div class='row'>";

				dir_content += "<div class='fn'><a href='";
				dir_content += dp->d_name;
				if (tmp != NULL)
					dir_content += "/";
				dir_content += "'>";
				dir_content += dp->d_name;
				if (tmp != NULL)
					dir_content += "/";
				dir_content += "</a></div>";

				stat(fn.c_str(), &fs);
				tm = localtime(&fs.st_mtime);
				strftime(dt, sizeof(dt), "%x %r", tm);
				dir_content += "<div class='tm'>";
				dir_content += dt;
				dir_content += "</div>";

				dir_content += "<div class='size'>";
				if (tmp != NULL)
					dir_content += "-";
				else if (stat(fn.c_str(), &fs) == 0)
				{
					std::string	unit;
					size_t		size = fs.st_size;

					if (size > 1000000000000)
					{
						size /= 1000000000;
						unit = "g";
					}
					else if (size > 1000000000)
					{
						size /= 1000000;
						unit = "m";
					}
					else if (size > 1000000)
					{
						size /= 1000;
						unit = "k";
					}
					else
						unit = "";
					dir_content += std::to_string(size);
					dir_content += unit;
				}
				dir_content += "</div>";

				dir_content += "</div>";

				if (tmp != NULL)
					closedir(tmp);
			}
			dp = readdir(dir);
		}
		dir_content += "</div></body></html>";

		this->_data += "Content-Length: ";
		this->_data += std::to_string(dir_content.length());
		this->_data += "\r\n\r\n";
		this->_data += dir_content;
		closedir(dir);
	}
	else 
		this->parse_error_pages("403", "Forbidden", virtual_server);
}

void Response::handle_return(std::string return_route) {
	std::string return_content = "<html>\n<head><title>307 Temporary Redirect</title></head>\n<body>\n<center><h1>307 Temporary Redirect</h1></center>\n</body>\n</html>";

	this->_data += "HTTP/1.1 307 Temporary Redirect\r\n";
	this->_data += "Location: " + return_route + "\r\n";
	this->_data += "Content-Type: text/html\r\n";
	this->_data += "Content-Length: " + std::to_string(return_content.length()) + "\r\n";
	this->_data += "Location: " + return_route + "\r\n";
	this->_data += "\r\n";
	this->_data += return_content;
	this->_data += "\r\n";
	
}

void Response::handle_DELETE(std::string path, Server & virtual_server) {
	if (remove(path.c_str()) == -1) {
		if (errno == ENOENT) 
			this->parse_error_pages("404", "Not Found", virtual_server);
		else
			this->parse_error_pages("500", "Internal Server Error", virtual_server);
	}
	else {
		std::string response_content = "<html>\n<head><title>200 OK</title></head>\n<body>\n<center><h1>200 OK</h1></center>\n</body>\n</html>";
		this->_data += "HTTP/1.1 200 OK\r\n";
		this->_data += "Content-Type: text/html\r\n";
		this->_data += "Content-Length: " + std::to_string(response_content.length()) + "\r\n";
		this->_data += "\r\n";
		this->_data += response_content;
		this->_data += "\r\n";
	}

}

void	Response::parse_GET_method(Request & request, Server & virtual_server) {
	Location * location = this->parse_location(request, virtual_server);
	
	if (location == NULL)
		this->parse_error_pages("403", "Forbidden", virtual_server);
	else {
		std::string resource_path = parse_resource_path(request, *location);
		if ((*location).get_return() != "") 
			this->handle_return((*location).get_return());
		else if (resource_path[resource_path.length() - 1] == '/') {
			if (location->get_index().size() == 0) {
				if (location->get_autoindex())
					this->parse_autoindex(resource_path, request.get_route(), virtual_server);
				else
					this->parse_error_pages("404", "Not Found", virtual_server);
			}
			else {
				std::vector<std::string>::iterator it = location->get_index().begin();
				std::vector<std::string>::iterator ite = location->get_index().end();
				for (; it != ite; it++) {
					if (this->_data == "") 
						this->parse_resource(resource_path + *it);
				}
				if (this->_data == "")
					this->parse_error_pages("404", "Not Found", virtual_server);
			}
		}
		else {
			this->parse_resource(resource_path);
			if (this->_data == "")
				this->parse_error_pages("404", "Not Found", virtual_server);
		}
	}
}

void	Response::parse_POST_method(Request & request, Server & virtual_server) {
	Location * location = this->parse_location(request, virtual_server);

	if (location == NULL)
		this->parse_error_pages("403", "Forbidden", virtual_server);
	else {
		std::string resource_path = parse_resource_path(request, *location);
		if ((*location).get_return() != "") 
			this->handle_return((*location).get_return());
		else if (resource_path[resource_path.length() - 1] == '/')
				this->parse_error_pages("403", "Forbidden", virtual_server);
		else {
			if (location->get_cgi_pass() == "")
				this->parse_error_pages("405", "Method not allowed ", virtual_server);
			else if (request.get_content_length() != "" && virtual_server.get_client_max_body_size() != "" && \
			std::stoll(request.get_content_length()) > std::stoll(virtual_server.get_client_max_body_size()))
				this->parse_error_pages("413", "Payload Too Large", virtual_server);
			else {
				//SEARCH FOR CGI_PASS DIRECTORY (IF NOT ERROR 404 NOT FOUND)
				std::cout << "HANDLE CGI" << std::endl;
			}
		}
	}
}

void	Response::parse_DELETE_method(Request & request, Server & virtual_server) {
	Location * location = this->parse_location(request, virtual_server);

	if (location == NULL)
		this->parse_error_pages("403", "Forbidden", virtual_server);
	else {
		std::string resource_path = parse_resource_path(request, *location);
		if ((*location).get_return() != "") 
			this->handle_return((*location).get_return());
		else {
			std::vector<std::string>::iterator it = location->get_allowed_methods().begin();
			std::vector<std::string>::iterator ite = location->get_allowed_methods().end();
			if (std::find(it, ite, "DELETE") == ite)
				this->parse_error_pages("405", "Method not allowed", virtual_server);
			else
				this->handle_DELETE(resource_path, virtual_server);
		}
	}
}

void	Response::parse_response_data(Request & request, std::vector<Server> & servers)
{
	Server & virtual_server = parse_virtual_server(request, servers);
	
	if (request.get_method() == "GET")
		this->parse_GET_method(request, virtual_server);
	else if (request.get_method() == "POST")
		this->parse_POST_method(request, virtual_server);
	else if (request.get_method() == "DELETE")
		this->parse_DELETE_method(request, virtual_server);
	else
		this->parse_error_pages("405", "Method Not Allowed", virtual_server);
}