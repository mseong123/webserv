#include "cgi.hpp"

std::string urlDecode(const std::string& input) {
    std::ostringstream decoded;
    decoded.fill('0');

    for (size_t i = 0; i < input.length(); ++i) {
        char ch = input[i];
        if (ch == '%' && i + 2 < input.length()) {
            // Found a percent-encoded sequence
            char hex1 = input[++i];
            char hex2 = input[++i];

            // Convert hexadecimal characters to integer
            std::istringstream hexStream("0x" + std::string(1, hex1) + std::string(1, hex2));
            int value;
            hexStream >> std::hex >> value;

            // Append the decoded character to the result
            decoded << static_cast<char>(value);
        } else if (ch == '+') {
            // Replace '+' with space
            decoded << ' ';
        } else {
            // Append other characters as they are
            decoded << ch;
        }
    }

    return decoded.str();
}


std::string parse_boundary(std::string message_body) {
	std::string content_type = getenv("CONTENT_TYPE");
	std::string boundary;
	size_t pos = 0;
	size_t pos_boundary = 0;
	size_t temp_boundary = 0;
	std::string parsed_message_body;

	pos = content_type.find("boundary=");
	pos += 9;
	boundary = content_type.substr(pos, std::string::npos);
	boundary = boundary.substr(0, boundary.length()-1);
	boundary = "--" + boundary;
	pos_boundary = message_body.find(boundary);
	temp_boundary = message_body.find("\r\n", pos_boundary);
	parsed_message_body = message_body.substr(temp_boundary + 2, std::string::npos);
	pos_boundary = parsed_message_body.find(boundary);
	parsed_message_body = parsed_message_body.substr(0, pos_boundary);
	return parsed_message_body;
}

void parse_message_body(std::string message_body, std::string curr_direct) {
	size_t pos;
	size_t temp_pos;

	std::string parse_message_body = parse_boundary(message_body);
	pos = parse_message_body.find("filename=");
	if (pos == std::string::npos) {
		std::cout << "Internal script error (Can't find filename in message body)" << std::endl;
		std::cout << "No File uploaded" << std::endl;
		exit(EXIT_FAILURE);
	}
	else {
		std::string filename;
		pos += 10;
		temp_pos = parse_message_body.find_first_of("\"", pos);
		if (temp_pos != std::string::npos)
			filename = parse_message_body.substr(pos, temp_pos - pos);
		// std::ofstream outputFile(curr_direct + filename);
		std::ofstream outputFile(curr_direct + filename);
		if (outputFile.is_open()) {
			pos = temp_pos;
			int start = parse_message_body.find("\r\n\r\n");
			start += 4;
			outputFile << parse_message_body.substr(start, std::string::npos) << std::endl;
			outputFile.close();
			std::cout << "<html><body style='font-family: Arial; sans-serif;margin: 20px;'>";
			std::cout << "<div>File uploaded successfully.</div>";
			std::cout << "<div>Uploaded File: ";
			std::cout << filename;
			std::cout << "</div>";
			std::cout << "</body></html>";
		}
		else {
			std::cout << "Internal script error (Can't create file)" << std::endl;
			std::cout << "No File uploaded" << std::endl;
			exit(EXIT_FAILURE);
		}
	}
}


void parse_resource(std::string route) {
	std::fstream	resource(route);
	if (resource.is_open()) {
		
		std::string	resource_content;
		std::string	resouce_type;
		std::string line;

		while (getline(resource, line))
			resource_content = resource_content + "\n" + line;
		
		resource.close();
		resouce_type = route.substr(route.find_last_of(".") + 1);
		std::cout << "HTTP/1.1 200 OK\r\n";
		std::cout << "Content-Type: ";
		if ((resouce_type.compare("html") == 0 || resouce_type.compare("txt") == 0) && resource_content.length() < 50000) {
			if (resouce_type.compare("html") == 0)
				std::cout << "text/html";
			else
				std::cout << "text/plain";
			std::cout << "\r\n";
			std::cout << "Content-Length: ";
			std::cout << std::to_string(resource_content.length()) + "\r\n";
			std::cout << resource_content;
			std::cout << "\r\n";
		}
		else {
			std::cout << "Content-Type: text/plain\r\n";
			std::cout << "\r\n";
			std::cout << "Error: C++ CGI can only serve .html and .txt files and files < 50kb";
			std::cout << "\r\n";
		}
	}
	else {
		std::string error_message_body = "<html><head><title>404 Not Found(CGI - C++)</title></head><body><center><h1>404 Not Found(CGI-C++)</h1></center></body></html>";
		std::cout << "HTTP/1.1 404 Not Found(CGI-C++)\r\n";
		std::cout << "Content-Type: text/html\r\n";
		std::cout << std::string("Content-Length: ") + std::to_string(error_message_body.length()) + "\r\n";
		std::cout << "\r\n";
		std::cout << error_message_body;
		std::cout << "\r\n";
	}
}


int main(int argc, char **argv) {
	if (std::string(getenv("REQUEST_METHOD")) == "POST") {
		char buffer[1024];
		std::string message_body;
		int rec_byte;
		std::string curr_direct;
		unsigned long content_length;

		if (argc == 2)
			curr_direct = std::string(argv[1]);
		else {
			std::cout << "Internal script error (Invalid no of arguments)" << std::endl;
			std::cout << "No File uploaded" << std::endl;
			exit(EXIT_FAILURE);
		}
		while ((rec_byte = read(0, buffer, 1024 - 2)) != 0) {
			message_body.append(buffer, rec_byte);
		}
		if (getenv("CONTENT_LENGTH") != NULL) {
			try {
				content_length = std::stoull(getenv("CONTENT_LENGTH"));
			}
			catch (std::exception & e) {
				std::cout << "Internal script error (Invalid CONTENT_LENGTH header)" << std::endl;
				std::cout << "No File uploaded" << std::endl;
				exit(EXIT_FAILURE);
			}
			if (message_body.length() != content_length) {
				std::cout << "Internal script error (received content length does not match header CONTENT_LENGTH)" << std::endl;
				std::cout << "No File uploaded" << std::endl;
				exit(EXIT_FAILURE);
			}
			else {
				std::string boundary;
				
				if (getenv("CONTENT_TYPE") != NULL && std::string(getenv("CONTENT_TYPE")).find("boundary") != std::string::npos)
					parse_message_body(message_body, curr_direct);
				else {
					std::cout << "Internal script error (CONTENT_TYPE header does not include boundary)" << std::endl;
					std::cout << "No File uploaded" << std::endl;
					exit(EXIT_FAILURE);
				}
			}
		}
		else {
			std::cout << "Internal script error (No CONTENT_LENGTH header)" << std::endl;
			std::cout << "No File uploaded" << std::endl;
			exit(EXIT_FAILURE);
		}
		
	}
	else if (std::string(getenv("REQUEST_METHOD")) == "GET"){
		std::string encodedString = std::string(getenv("ROUTE"));
    	std::string decodedString = urlDecode(encodedString);

		parse_resource("." + decodedString);
	}

		
	return (0);
}