#include "Compiler.hpp"
#include <set>

using namespace Florence::FSCompiler;

const std::set<std::string> reserved_labels {"exit", "main_menu", "about", "stdin", "stdout",
                                             "stderr", "set_graphics_mode", "draw_image", "write_screen",
                                             "cr", "lf", "semicolon", "format", "sleep", "sleep_loop",
                                             "sleep_exit", "choose", "jump_to_one", "jump_to_three",
                                             "print_char", "print", "print_loop", "print_loop_no_sleep",
                                             "print_ret"};

/**
 * Convert a string to be compatible to the x86. This includes,
 *  converting ";" to semicolon constant, converting " to '"',
 *  and converting newlines to cr, lf pairs.
 *
 * @param string_ String to convert.
 * @param null_terminated: If set to true, the string is null terminated rather than $.
 *  this string type is used by some BIOS and graphics routines.
 * @return The converted string.
 */
std::string convert_string(std::string string_, bool null_terminated) {
    std::string converted_string = "\"";
    for (char const &c : string_) {
        switch (c) {
            case '"':
                converted_string += "\", '\"', \""; // Replace and wrap with single quote.
                break;
            case ';':
                converted_string += "\", semicolon, \""; // Replaces with reference to constant.
                break;
            case '\n':
                converted_string += "\", cr, lf, \"";
                break;
            default:
                converted_string += c;
        }
    }
    converted_string += null_terminated ? "\", 0" : "$\""; // $ terminated string.
    return converted_string;
}
/**
 * Check if the keyword is reserved.
 *
 * @param name Name to check for.
 * @return True if keyword is reserved.
 */
bool is_reserved(std::string name) {
    return reserved_labels.find(name) != reserved_labels.end();
}

/* Compiles to the data segment. */
void Compiler::compile_element(Tokens::GlobalToken *token, bool add_suffix, bool null_terminated) {
    if (is_reserved(token->key)) {
        // Emit an error for the reserved keyword usage.
        emit_error(FlorenceError::Error::COMPILER_RESERVED, token->line, token->location, "This keyword is reserved.");
    } else {
        this->data_segment_ +=
                token->key + (add_suffix ? "_text": "") + "\tdb\t" + convert_string(token->value, null_terminated) + "\n"; // Add to data segment.
    }
    delete token; // Free the token.
}

void Compiler::compile_element(IRElements::GlobalsIRElement *element) {
    Tokens::GlobalToken *global_token = nullptr;
    while (!element->globals.empty()) {
        global_token = element->globals.front();
        element->globals.pop(); // Dequeue the element.
        compile_element(global_token, false, false); // Compile the token.
    }
}

void Compiler::compile_element(IRElements::SectionIRElement *element) {
    // This function only compiles the text to the data segment.
    if (this->labels_.find(element->section_label->label) != this->labels_.end()) { // If label already exists.
        emit_error(FlorenceError::Error::COMPILER_LABEL_REDEFINED, element->section_label->line,
                   element->section_label->location, "Label cannot be defined more than once.");
        return;
    }
    auto *global_token = new Tokens::GlobalToken {Tokens::TokenType::GLOBAL, element->section_label->line,
                                                  element->section_label->location, element->section_label->label,
                                                  element->text}; // Convert to global token to reuse compile_element.
    compile_element(global_token, true, element->element_type == IRElements::IRElementType::IMAGE_SECTION_IR_ELEMENT);
    this->labels_.insert(element->section_label->label); // Add the label to the labels set.
}

/* Compiles to the logic segment. */
void Compiler::compile_element(IRElements::TextSectionIRElement *element) {
    std::string generated_assembly;
    std::string label_name = element->section_label->label;
    generated_assembly +=  label_name + ":\n"; // Generate label.
    generated_assembly += "\tlea\tax, [" + label_name + "_text]\n"; // Put the to ax.
    generated_assembly += "\tcall\tprint\n"; // Call the print function.
    std::string registers[3] = {"ax", "bx", "cx"}; // Registers
    std::string jump_name; // Traversal for jump names.
    for (const std::string &r : registers) {
        if (element->choices.empty()) {
            break;
        }
        jump_name = element->choices.front()->choice;
        element->choices.pop();
        generated_assembly += "\tlea\t" + r + ", [" + jump_name + "]\n";
    }
    generated_assembly += "\tjmp\tchoose\n";
    this->logic_segment_ += generated_assembly; // Add the generated assembly
    delete element; // Free the element.
}

void Compiler::compile_element(IRElements::ImageSectionIRElement *element) {
    std::string generated_assembly;
    std::string label_name = element->section_label->label;
    generated_assembly += label_name + ":\n"; // Generate label.
    generated_assembly += "\tlea\tax, [" + label_name + "_text]\n"; // Put the file name handler.
    generated_assembly += "\tcall\tdraw_image\n"; // Call the draw image function.
    generated_assembly += "\tjmp\t" + element->direct->direction + "\n"; // Set the jump location.
    delete element;
    this->logic_segment_ += generated_assembly;
}

void Compiler::compile_element(IRElements::DirectSectionIRElement *element) {
    std::string generated_assembly;
    std::string  label_name = element->section_label->label; // Get the label name.
    generated_assembly += label_name + ":\n"; // Generate the label.
    generated_assembly += "\tlea\tax, [" + label_name + "_text]\n"; // Print the text.
    generated_assembly += "\tcall\tprint\n"; // Print the text.
    generated_assembly += "\tjmp\t" + element->direct->direction + "\n"; // Jump to next direction.
    delete element;
    this->logic_segment_ += generated_assembly;
}

void Compiler::compile_sections() {
    while (peek() != nullptr) {
        auto *element = (IRElements::SectionIRElement *) dequeue();
        switch (element->element_type) { // Compile the sections one by one.
            case IRElements::GLOBALS_IR_ELEMENT:
                emit_error(FlorenceError::Error::COMPILER_MISPLACED_GLOBALS, 0, 0, "Misplaced global detected.");
                break;
            case IRElements::SECTION_IR_ELEMENT:
                compile_element((IRElements::TextSectionIRElement *) element);
                break;
            case IRElements::IMAGE_SECTION_IR_ELEMENT:
                compile_element((IRElements::ImageSectionIRElement *) element);
                break;
            case IRElements::DIRECT_SECTION_IR_ELEMENT:
                compile_element((IRElements::DirectSectionIRElement *) element);
                break;
        }
    }
}

void Compiler::compile_data_segment() {
    int length = this->intermediate_representation_.size();
    auto *globals_ir_element = (IRElements::GlobalsIRElement *) dequeue(); // Get the globals.
    if (globals_ir_element == nullptr) {
        emit_error(FlorenceError::Error::COMPILER_MISSING_GLOBALS, 0, 0, "Missing globals section.");
    }
    compile_element(globals_ir_element); // Compile the globals.
    IRElements::SectionIRElement *section_ir_element;
    for (int i = 1; i < length; i++) {
        IRElements::IRElement *ir_element = peek();
        if (ir_element->element_type == IRElements::GLOBALS_IR_ELEMENT) {
            emit_error(FlorenceError::COMPILER_MISPLACED_GLOBALS, 0, 0, "Misplaced globals section.");
            break;
        }
        section_ir_element = (IRElements::SectionIRElement*) dequeue(); // Dequeue the element.
        compile_element(section_ir_element);  // Add the text.
        this->intermediate_representation_.push(section_ir_element); // Push back to the last part.
    }
}

void Compiler::emit_error(FlorenceError::Error error_type, int row, int column, std::string error_message) {
    this->error_logger_->emit_error(error_type, row, column, error_message); // Emit an error.
}

IRElements::IRElement *Compiler::dequeue() {
    IRElements::IRElement *next_element = peek();
    if (next_element != nullptr) { // If the IR queue is not empty.
        this->intermediate_representation_.pop(); // Remove the element from the front.
    }
    return next_element; // Return the next element (or nullptr).
}

IRElements::IRElement *Compiler::peek() {
    if (this->intermediate_representation_.empty()) {
        return nullptr; // If empty, return nullptr.
    }
    return this->intermediate_representation_.front();
}
std::string Compiler::compile() {
    compile_data_segment(); // Compile the data segment.
    if (this->error_logger_->has_error_occurred()) {
        return "";
    }
    compile_sections(); // Compile the sections.
    return this->logic_segment_ + '\n' + this->data_segment_; // Combine two segments.
}