#include<stdio>
#include "ErrorLogger.hpp"

bool has_error_occurred_ = false; // Initially false.
	
void ErrorLogger::emit_error(FlorenceError::Error error_type, int row, int column, std::string error_message) {
	this.has_error_occured = true; // An error has occured!
	std::stderr << "An error has occured in" << row << ", " << column << ": " << error_message << "\n";
}
