/**
* MIT LICENSE - 2021 - ADA86 Games
* @author Ambertide
* This module contains token types.
*/
#include <string>

namespace Tokens {
	enum TokenType {
		LABEL, // Used by section labels.
		DIRECT, // Used by direct jumps.
		BLOCK, // Used by string blocks.
		GLOBAL, // Used by global variables.
		CHOICE // Used by choice jumps.
	};

	/**
	* A structure used for representing tokens
	*	that are emitted from the tokenizer.
	*	holds data about location and line of
	*	the token as well for error reporting.
	*/
	struct Token {
		TokenType token_type; /** Type of the token. */
		int line; /** Line number of the Token. */
		int location; /** Location of the Token */
	};

	/**
	* A structure used by the LabelTokens.
	*	these tokens are labels used by the
	*	labels of the sections.
	*/
	struct LabelToken : Token {
		bool is_image; /** True if it is a label for an image section. */
		std::string label; /** Actual label as used by */ 
	};

	/**
	* Represent a direct jump to a section,
	* 	either directly, or in the image sections,
	*	after a key press.
	*/
	struct DirectToken : Token {
		std::string direction; /** Label name to jump to. */
	};

	/**
	* Represents a choice in the section,
	* 	upon key press, the choice of the
	*	user will jump to the choice.
	*/
	struct ChoiceToken : Token {
		std::string choice;
	};

	/**
	* Represents a string block inside the
	* 	sections.
	*/
	struct BlockToken : Token {
		std::string text; /** Text of the string block. */
	};

	/**
	* Repersents a global variable in the program.
	*	these are placed inside the data segment.
	*/
	struct GlobalToken : Token {
		std::string key; /** Key of the token. Name of the variable. */
		std::string value; /** Value of the variable. */
	};
};
