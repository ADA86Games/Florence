#ifndef FLORENCE_TOKENIZER_HPP
#define FLORENCE_TOKENIZER_HPP
#include<string>
#include<queue>
#include "Tokens.hpp"
#include "ErrorLogger.hpp"
#include "FlorenceError.hpp"

/**
* Tokenizer class that emits Tokens
* 	from the given source code.
*/
class Tokenizer {
	private:
	std::string source_code_; /** Source code to traverse. */
	std::queue<Tokens::Token> token_queue_; /** Token queue to emit tokens to. */
	int current_line_; /** Current line the tokenizer is in. */
	int current_col_; /** Current row the tokenizer is in. */
	int current_char_; /** Current character */
	ErrorLogger *logger_; /** Error Logger to log errors. */
	
	/**
	* Emit a BlockToken. This represents a string inside
	* 	a section.
	*/
	void emit_block();

	/**
	* Ignore comments. Comments are "deleted". Empty lines are also
	* 	deleted.
	*/
	void emit_comment();

	/**
	* Emit a global variable token, global variables are placed to
	*	the data segment.
	*/
	void emit_global();
	
	/**
	* Emit a label for a section.
	*/
	void emit_label();
	
	/**
	* Emit a direct jump token.
	*/
	void emit_direct();
	
	/**
	* Emit a choice jump token.
	*/
	void emit_choice();
	
	/**
	* Emit an error, uses the Error logger.
	*
	* @param error: Type of the error.
	*/
	void emit_error(FlorenceError::Error error);
	
	/**
	* Get a string from the current position.
	*
	* @param terminating_characters: Characters that terminate a string.
	* @param invalid_characters: If these characters are seen, emit an error.
	* @return the string.
	*/
	std::string get_string(const char *terminating_characters, const char *invalid_characters);

	/**
	 * Get a string from the current position without any invalid characters.
	 *
	 * @param terminating_characters: Characters that terminate a string.
	 * @return the string.
	 */
	std::string get_string(const char *terminating_characters);
	
	/**
	* Dequeue the next character.
	*
	* @return The next character.
	*/
	char dequeue();

	/**
	* Peek to see the next characters, without removing it.
	*
	* @return the nexet character
	*/
	char peek();

	/**
	 * Rewind the queue back.
	 */
	inline void rewind();
	
	public:
	/**
	* Construct a tokenizer object.
	*
	* @param source_code: Source code of the program.
	*/
	Tokenizer(std::string &source_code, ErrorLogger *logger) : source_code_(source_code), logger_(logger) {
		this->current_char_ = 0;
		this->current_line_ = 0;
		this->current_col_ = 0;
	};
	
	/**
	* Tokenize the source code and return the
	* 	token queue.
	*
	* @return: the queue containing the tokens.
	*/
	std::queue<Tokens::Token> tokenize();
};
#endif // FLORENCE_TOKENIZER_HPP
