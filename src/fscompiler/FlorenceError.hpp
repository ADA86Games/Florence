#ifndef FLORENCE_ERROR_HPP
#define FLORENCE_ERROR_HPP

namespace Florence::FSCompiler::FlorenceError { 
	enum Error {
		UNEXPECTED_LEXEME,
		PARSER_NO_FILENAME,
		PARSER_NO_DIRECTION,
		PARSER_NO_LABEL
	};
};

#endif // FLORENCE_ERROR_HPP