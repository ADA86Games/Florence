#include "Parser.hpp"

using namespace Florence::FSCompiler;

std::queue<IRElements::IRElement*> Parser::parse() {
    parse_globals(); // Parse the globals.
    if (!this->error_logger_->has_error_occurred()) {
        parse_sections(); // Parse all sections if no error has occurred.
    }
    return this->intermediate_representation_;
}

void Parser::parse_globals() {
    auto *globals_ir_element = new IRElements::GlobalsIRElement();
    globals_ir_element->element_type = IRElements::IRElementType::GLOBALS_IR_ELEMENT; // Set type.
    while (peek()->token_type == Tokens::TokenType::GLOBAL) { // While the token is a global.
        globals_ir_element->globals.push((Tokens::GlobalToken*) dequeue()); // Push token to the queue.
    }
    this->intermediate_representation_.push(globals_ir_element); // Enqueue the globals IRElement.
}

void Parser::parse_sections() {
    Tokens::Token *token;
    while ((token = peek()) != nullptr) { // While the list is not empty.
        if (token->token_type != Tokens::TokenType::LABEL) { // If not label.
            emit_error(FlorenceError::PARSER_NO_LABEL, token->line, token->location, "Section without label."); // Emit error.
        }
        parse_section(); // Parse a section.
    }
}

void Parser::parse_section() {
    if (((Tokens::LabelToken*) peek())->is_image) {
        parse_image_section(); // Parse an image section.
        return;
    }
    auto *section = new IRElements::TextSectionIRElement();
    section->section_label = (Tokens::LabelToken*) dequeue();
    section->element_type = IRElements::IRElementType::SECTION_IR_ELEMENT;
    while (peek() != nullptr && peek()->token_type == Tokens::TokenType::BLOCK) { // If it is a string block.
        auto *block_token = (Tokens::BlockToken*) dequeue();
        section->text += block_token->text; // Get the text of the section, if any.
        delete block_token; // Free the block tokens.
    }
    Tokens::Token *token = dequeue(); // Either a choice or a direct.
    if (token != nullptr && token->token_type == Tokens::TokenType::DIRECT) {
        auto *direct_section = new IRElements::DirectSectionIRElement();
        direct_section->section_label = section->section_label;
        direct_section->text = section->text; // Transfer information.
        direct_section->element_type = IRElements::IRElementType::DIRECT_SECTION_IR_ELEMENT;
        direct_section->direct = (Tokens::DirectToken*) token; // Set the choice.
        this->intermediate_representation_.push(direct_section);
    } else if (token != nullptr && token->token_type == Tokens::TokenType::CHOICE) {
        section->choices.push((Tokens::ChoiceToken*) token); // Enqueue the first choice.
        while (peek()->token_type == Tokens::TokenType::CHOICE) {
            section->choices.push((Tokens::ChoiceToken*) dequeue()); // Enqueue rest of the choices.
        }
        this->intermediate_representation_.push(section); // Push the section.
    } else {
        emit_error(FlorenceError::Error::PARSER_NO_DIRECTION, section->section_label->line,
                    section->section_label->location, "Section without choice or direct jump.");
    }
}

void Parser::parse_image_section() {
    auto *section = new IRElements::ImageSectionIRElement();
    section->section_label = (Tokens::LabelToken*) dequeue(); // Get the label.
    if (peek()->token_type == Tokens::TokenType::BLOCK) {
        auto *block_filename = ((Tokens::BlockToken *) dequeue());
        section->filename = block_filename->text;
        delete block_filename; // Free the block.
    } else {
        emit_error(FlorenceError::Error::PARSER_NO_FILENAME, section->section_label->line,
                    section->section_label->location, "Image without filename.");
        return;
    }
    if (peek()->token_type != Tokens::TokenType::DIRECT) {
        emit_error(FlorenceError::Error::PARSER_NO_DIRECTION, section->section_label->line,
                    section->section_label->location, "Image without next jump.");
        return;
    }
    section->direct = (Tokens::DirectToken*) dequeue();
    section->element_type = IRElements::IRElementType::IMAGE_SECTION_IR_ELEMENT; // Set type.
    this->intermediate_representation_.push(section);
}

void Parser::emit_error(FlorenceError::Error error, int row, int col, std::string error_message) {
    this->error_logger_->emit_error(error, row, col, error_message);
}

Tokens::Token *Parser::dequeue() {
    Tokens::Token *token = peek();
    if (token != nullptr) {
        token_queue_.pop();
    }
    return token;
}

Tokens::Token *Parser::peek() {
    Tokens::Token *token = nullptr;
    if (!this->token_queue_.empty()) {
        token = token_queue_.front();
    }
    return token;
}