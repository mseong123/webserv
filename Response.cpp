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
	for (; it != ite; it++) {
		if (it->get_route() == req_path.substr(0, it->get_route().length()))
			return &(*it);
	}
	return NULL;
}

std::string	Response::parse_resource_path(Request & request, Location & location) {
	std::string resource_path;

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

void Response::handle_DELETE(std::string path) {
	(void)path;
}



void	Response::parse_GET_method(Request & request, Server & virtual_server) {
	Location * location = this->parse_location(request, virtual_server);
	
	if (location == NULL)
		this->parse_error_pages("403", "Forbidden", virtual_server);
	else {
		std::string resource_path = parse_resource_path(request, *location);
		if (resource_path[resource_path.length() - 1] == '/') {
			if (location->get_index().size() == 0) 
				this->parse_error_pages("404", "Not Found", virtual_server);
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
		if (resource_path[resource_path.length() - 1] == '/')
				this->parse_error_pages("403", "Forbidden", virtual_server);
		else {
			if (location->get_cgi_pass() == "")
				this->parse_error_pages("403", "Forbidden", virtual_server);
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
		if (resource_path[resource_path.length() - 1] == '/')
				this->parse_error_pages("403", "Forbidden", virtual_server);
		else {
			std::vector<std::string>::iterator it = location->get_allowed_methods().begin();
			std::vector<std::string>::iterator ite = location->get_allowed_methods().end();
			if (std::find(it, ite, "DELETE") == ite)
				this->parse_error_pages("405", "METHOD NOT ALLOWED", virtual_server);
			else
				this->handle_DELETE(resource_path);
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