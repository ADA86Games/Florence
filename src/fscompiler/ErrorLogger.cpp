#include<iostream>
#include "ErrorLogger.hpp"
	
void Florence::FSCompiler::ErrorLogger::emit_error(FlorenceError::Error error_type, int row, int column, std::string error_message) {
	this->has_error_occurred_ = true; // An error has occured!
	std::cerr << "An error has occured in" << row << ", " << column << ": " << error_message << "\n";
}

bool Florence::FSCompiler::ErrorLogger::has_error_occurred() {
	return this->has_error_occurred_;
}