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

void Florence::FSCompiler::Tokenizer::emit_block() {
    char next_char; // Traversing char.
    std::string block_text; // String to append to.
    auto *block = new Tokens::BlockToken();  // Block token object to generate.
    block->line = this->current_line_; // Set token location.
    block->location = this->current_col_;
    block->token_type = Tokens::TokenType::BLOCK; // Set type.
    rewind(); // Go back a single character.
    while(true) {
        if (peek() == '^') { // If special ignore character.
            dequeue(); // Toss away the character.
            dequeue(); // Toss away the next character as well.
        }
        next_char = dequeue();
        if (next_char == '\n' && one_of(peek(), "*:[>;")) { // Break condition.
            break;
        }
        block_text += next_char;
    }
    block->text = block_text;
    token_queue_.push(block); // Enqueue the token.
}

void Florence::FSCompiler::Tokenizer::emit_comment() {
    while (dequeue() != '\n'); // Toss away all including EOL.
}

void Florence::FSCompiler::Tokenizer::emit_global() {
    auto *global_token = new Tokens::GlobalToken();
    global_token->line = this->current_line_;
    global_token->location = this->current_col_;
    std::string key = get_string(":", "\n");
    if (this->logger_->has_error_occurred()) { // Stop if any error has occurred.
        return;
    }
    std::string value = get_string("\n");
    /** Set variables. */
    global_token->token_type = Tokens::TokenType::GLOBAL;
    global_token->key = key;
    global_token->value = value;
    this->token_queue_.push(global_token); // Enqueue for compilation.
}

void Florence::FSCompiler::Tokenizer::emit_label() {
    auto *label = new Tokens::LabelToken();
    label->line = this->current_line_; // Current location
    label->location = this->current_col_; // Is the label location.
    label->token_type = Tokens::TokenType::LABEL;
    label->is_image = false; // Initially false.
    if (peek() == '!') { // Indicates image label.
        label->is_image = true; // Set to true.
        dequeue(); // Discard !.
    }
    label->label = get_string("]", "\n"); // Get the label text.
    this->token_queue_.push(label);  // Enqueue a label token.
}

void Florence::FSCompiler::Tokenizer::emit_direct() {
    auto *token = new Tokens::DirectToken();
    token->line = this->current_line_;
    token->location = this->current_col_;
    token->token_type = Tokens::TokenType::DIRECT;
    token->direction = get_string("\n"); // Get the jump label.
    token_queue_.push(token); // Push the token.
}

void Florence::FSCompiler::Tokenizer::emit_choice() {
    auto *token = new Tokens::ChoiceToken();
    token->line = this->current_line_;
    token->location = this->current_col_;
    token->token_type = Tokens::TokenType::CHOICE;
    token->choice = get_string("\n");
    token_queue_.push(token); // Push the token.
}

void Florence::FSCompiler::Tokenizer::emit_error(FlorenceError::Error error) {
    this->logger_->emit_error(error, this->current_line_, this->current_col_, "Unexpected character.");
}

std::string Florence::FSCompiler::Tokenizer::get_string(const char *terminating_characters, const char *invalid_characters) {
    std::string str_ = "";
    char c;
    while (true) {
        c = dequeue();
        if (one_of(c, invalid_characters)) {
            emit_error(FlorenceError::UNEXPECTED_LEXEME); // Emit error if character is unexpected.
            return "";
        } else if(one_of(c, terminating_characters)) {
            return str_; // Terminating character is not included.
        }
        str_ += c; // Append to the string.
    }
    return str_;
}

std::string Florence::FSCompiler::Tokenizer::get_string(const char *terminating_characters) {
    return get_string(terminating_characters, ""); // Call get string without error characters.
}

char Florence::FSCompiler::Tokenizer::dequeue() {
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

char Florence::FSCompiler::Tokenizer::peek() {
    return this->source_code_[this->current_char_]; // Get the current character.
}

inline void Florence::FSCompiler::Tokenizer::rewind() {
    this->current_char_--; // Go back.
    this->current_line_--; // This only occurs in blocks.
}

std::queue<Florence::FSCompiler::Tokens::Token*> Florence::FSCompiler::Tokenizer::tokenize() {
    while (peek() && !this->logger_->has_error_occurred()) {
        char next_char = dequeue();
        switch (next_char) { // Switch case on syntactic elements.
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
        case '\n':
            break; // Ignore the newline.
        default:
            emit_block();
            break;
        }
    }
    return this->token_queue_;
}