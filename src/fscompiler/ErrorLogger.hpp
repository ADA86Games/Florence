#ifndef FSCOMPILER_ERRORLOGGER_HPP
#define FSCOMPILER_ERRORLOGGER_HPP
#include<string>
#include "FlorenceError.hpp"
namespace Florence::FSCompiler {
	class ErrorLogger {
		private:
		bool has_error_occurred_; /** Whether or not an error has occured yet. */

		public:
		/**
		* Emit an error to the stderr.
		*
		* @param error_type: Type of the error.
		* @param row: Row the error occured in.
		* @param column: Column the error occured in.
		* @param error_message: Error message to emit.
		*/
		void emit_error(FlorenceError::Error error_type, int row, int column, std::string error_message);
		
		/**
		 * Return if an error has occurred.
		*
		* @return True if an error has occurred.
		*/
		bool has_error_occurred();
	};
};
#endif // FSCOMPILER_ERRORLOGGER_HPP