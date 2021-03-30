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
            emit_error("Section wrongly configured."); // Emit error.
        }
        parse_section(); // Parse a section.
    }
}

void Parser::parse_section() {
    if (((Tokens::LabelToken*) peek())->is_image) {
        parse_image_section(); // Parse an image section.
    }
    auto *section = new IRElements::SectionIRElement();
    section->section_label = (Tokens::LabelToken*) dequeue();
    while (peek()->token_type == Tokens::TokenType::BLOCK) { // If it is a string block.
        section->text += ((Tokens::BlockToken*) dequeue())->text; // Get the text of the section, if any.
    }
    Tokens::Token *token = dequeue(); // Either a choice or a direct.
    if (token->token_type == Tokens::TokenType::DIRECT) {
        auto *direct_section = new IRElements::DirectSectionIRElement();
        direct_section->section_label = section->section_label;
        direct_section->text = section->text; // Transfer information.
        direct_section->element_type = IRElements::IRElementType::DIRECT_SECTION_IR_ELEMENT;
        direct_section->direct = (Tokens::DirectToken*) token; // Set the choice.
        this->intermediate_representation_.push(direct_section);
    } else if (token->token_type == Tokens::TokenType::CHOICE) {
        section->choices.push((Tokens::ChoiceToken*) token); // Enqueue the first choice.
        while (peek()->token_type == Tokens::TokenType::CHOICE) {
            section->choices.push((Tokens::ChoiceToken*) dequeue()); // Enqueue rest of the choices.
        }
        this->intermediate_representation_.push(section); // Push the section.
    } else {
        emit_error("Section without choice or direct jump.");
    }
}