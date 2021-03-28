#include <vector>
#include "Tokens.hpp"

/**
* Represents a TokenQueue holding tokens.
*	tokens are emitted to this queue from
*	the tokenizer, and removed from parser.
*/
class TokenQueue {
	private:
	std::vector<Tokens::Token> token_queue_; /** Represents the queue inside. */
	
	public:
	TokenQueue();
	/**
	* Enqueue a token inside the token queue.
	*
	* @param token: Token to enqueue.
	*/
	void enqueue(Tokens::Token token);

	/**
	* Dequeue a token and return it.
	*
	* @return the token at the front and remove it from the queue. 
	*/
	Tokens::Token dequeue();
};
