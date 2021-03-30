#include <iostream>
#include "fscompiler/Tokenizer.hpp"
#include "fscompiler/ErrorLogger.hpp"
#include "utils/fileio.hpp"

using namespace Florence::FSCompiler;

/**
 * Print a token.
 *
 * @param token Pointer to the token to print.
 * @return String representing token.
 */
std::string print_token(Tokens::Token *token) {
    Tokens::TokenType token_type = token->token_type;
    switch(token_type) {
        case Tokens::TokenType::LABEL: {
            auto *label = (Tokens::LabelToken *) token;
                return "Label Token\n\tname: " + label->label + "\n\ttype: " + (label->is_image ? "image" : "text");
            }
        case Tokens::TokenType::GLOBAL: {
                auto *global = (Tokens::GlobalToken*) token;
                return "Global Token\n\tname: " + global->key + "\n\tvalue: " + global->value;
            }
        case Tokens::TokenType::DIRECT: {
                auto *direct = (Tokens::DirectToken*) token;
                return "Direct Token\n\tjumps to: " +  direct->direction;
            }
        case Tokens::TokenType::CHOICE: {
                auto *choice = (Tokens::ChoiceToken*) token;
                return "Choice Token\n\tjumps to: " + choice->choice;
            }
        case Tokens::TokenType::BLOCK: {
                auto *block = (Tokens::BlockToken*) token;
                return "Block Token\n\tcontent: " + block->text;
            }
        default:
            return "Invalid Token";
    }
}

int main() {
    std::string source = Florence::Utils::read_source("../examples/example.flrnc");
    auto *logger = new ErrorLogger();
    auto *tokenizer = new Tokenizer(source, logger);
    std::queue<Tokens::Token*> tokens = tokenizer->tokenize();
    Tokens::Token *current_token;
    while (!tokens.empty()) {
        current_token = tokens.front();
        tokens.pop();
        std::cout << current_token->line + 1 << ", " << current_token->location + 1 << ": " << print_token(current_token) << "\n";
    }
}