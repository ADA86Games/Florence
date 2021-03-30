#include <iostream>
#include "fscompiler/Tokenizer.hpp"
#include "fscompiler/ErrorLogger.hpp"
#include "utils/fileio.hpp"
#include "fscompiler/Parser.hpp"
#include "fscompiler/Compiler.hpp"
#include "linker.hpp"

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

/**
 * Print a IRElement.
 *
 * @param token Pointer to the token to print.
 * @return String representing token.
 */
std::string print_block(IRElements::IRElement *element) {
    IRElements::IRElementType element_type = element->element_type;
    switch(element_type) {
        case IRElements::GLOBALS_IR_ELEMENT:
            return "Global Element";
        case IRElements::SECTION_IR_ELEMENT:
            return "Text Section with Choices Element: ";
        case IRElements::IMAGE_SECTION_IR_ELEMENT:
            return "Image Section with Direct Jump Element";
        case IRElements::DIRECT_SECTION_IR_ELEMENT:
            return "Text Section with Direct Jump Element";
        default:
            return "Undefined Element";
    }
}

int main() {
    std::string source = Florence::Utils::read_source("../examples/example.flrnc");
    auto *logger = new ErrorLogger();
    auto *tokenizer = new Tokenizer(source, logger);
    std::queue<Tokens::Token*> tokens = tokenizer->tokenize();
    auto *parser = new Parser(tokens, logger);
    std::queue<IRElements::IRElement*> ir = parser->parse();
    auto *compiler = new Compiler(ir, logger);
    std::string source_code = compiler->compile();
    std::string generated_assembly = Florence::Linker::link(source_code);
    Florence::Utils::write_source("../output.asm", generated_assembly);

}