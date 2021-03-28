#include<string>
#include "TokenQueue.hpp"
#include "ErrorLogger.hpp"
#include "FlorenceError.hpp"

/**
* Tokenizer class that emits Tokens
* 	from the given source code.
*/
class Tokenizer {
	private:
	std::string source_code_; /** Source code to traverse. */
	TokenQueue token_queue_; /** Token queue to emit tokens to. */
	int current_line_; /** Current line the tokenizer is in. */
	int current_row_; /** Current row the tokenizer is in. */
	const ErrorLogger &logger_; /** Error Logger to log errors. */
	
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
	
	public:
	/**
	* Construct a tokenizer object.
	*
	* @param source_code: Source code of the program.
	*/
	Tokenizer(std::string &source_code) : source_code_(source_code);
	/**
	* Tokenize the source code and return the
	* 	token queue.
	*
	* @return: the queue containing the tokens.
	*/
	TokenQueue tokenize();
}
