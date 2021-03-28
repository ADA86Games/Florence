#include "Tokenizer.hpp"

/**
 * Check if character is one of the characters
 * in the chars.
 * 
 * @param c: Character to search.
 * @param chars: Chars to search in.
 * @return true if c is in chars.
 */
inline bool one_of(char c, std::string chars) {
    return chars.find(c) != std::string::npos; // https://stackoverflow.com/a/43629706/6663851
}

void Tokenizer::emit_block() {
    char next_char; // Traversing char.
    std::string block_text = ""; // String to append to.
    Tokens::BlockToken block;  // Block token object to generate.
    block.line = this->current_line_; // Set token location.
    block.location = this->current_col_;
    block.token_type = Tokens::TokenType::BLOCK; // Set type.
    rewind(); // Go back a single character.
    while(true) {
        if (peek() == '^') { // If special ignore character.
            dequeue(); // Toss away the character.
        }
        next_char = dequeue();
        if (next_char == '\n' && one_of(peek(), "*:[>;")) { // Break condition.
            break;
        }
        block_text += next_char;
    }
    block.text = block_text;
    token_queue_.push(block); // Enqueue the token.
}

void Tokenizer::emit_comment() {
    while (dequeue() != '\n'); // Toss away all.
    dequeue(); // Toss away the EOL. 
}

void Tokenizer::emit_global() {
    Tokens::GlobalToken global_token;
    global_token.line = this->current_line_;
    global_token.location = this->current_col_;
    std::string key = get_string(":", "\n");
    if (this->logger_->has_error_occurred()) { // Stop if any error has occurred.
        return;
    }
    std::string value = get_string("\n");
    /** Set variables. */
    global_token.token_type = Tokens::TokenType::GLOBAL; 
    global_token.key = key;
    global_token.value = value;
    this->token_queue_.push(global_token); // Enqueue for compilation.
}

void Tokenizer::emit_label() {} // TODO: Implement this.
void Tokenizer::emit_direct() {} // TODO: Implement this.
void Tokenizer::emit_choice() {} // TODO: Implement this.

void Tokenizer::emit_error(FlorenceError::Error error) {
    this->logger_->emit_error(error, this->current_line_, this->current_col_, "Unexpected character.");
}

std::string Tokenizer::get_string(const char *terminating_characters, const char *invalid_characters) {} // TODO: Implement this.
std::string Tokenizer::get_string(const char *terminating_characters) {} // TODO: Implement this.

char Tokenizer::dequeue() {
    char character = peek();
    if (character != '\0' && character != '\n') { // If not at end of line or code.
        this->current_char_++; // Increment char.
        this->current_col_++; // Increment current row.
    } else if (character != '\0') { // If not End of code but EOL.
        this->current_char_++;
        this->current_line_++; // Increment line.
        this->current_col_ = 0; // Reset column.
    }
    return character; // Return the character.
}

char Tokenizer::peek() {
    return this->source_code_[this->current_char_]; // Get the current character.
}

inline void Tokenizer::rewind() {
    this->current_char_--; // Go back.
    this->current_line_--; // This only occurs in blocks.
}

std::queue<Tokens::Token> Tokenizer::tokenize() {
    while (!peek() && !this->logger_->has_error_occurred()) {
        char next_char = dequeue();
        switch (next_char) { // Switch case on syntaxtic elements.
        case '*': // Depending on the first character of the line.
            emit_choice();
            break;
        case ':':
            emit_direct();
            break;
        case ';':
            emit_comment();
            break;
        case '[':
            emit_label();
            break;
        case '>':
            emit_global();
            break;
        default:
            emit_block();
            break;
        }
    }
    return this->token_queue_;
}