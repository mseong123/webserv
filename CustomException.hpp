#ifndef CUSTOMEXCEPTION_HPP
#define CUSTOMEXCEPTION_HPP
#include <exception>
#include <iostream>

class CustomException : public std::exception {
	public:
		CustomException(std::string _message);
		~CustomException () _NOEXCEPT;
		const char *what() const throw();
	private:
		std::string _message;

};

#endif