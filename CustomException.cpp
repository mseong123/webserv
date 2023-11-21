#include "CustomException.hpp"

CustomException::CustomException(std::string message) : _message(message) {
};

CustomException::~CustomException () _NOEXCEPT {
};

const char *CustomException::what() const throw() {
	return this->_message.c_str();
};