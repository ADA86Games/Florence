#ifndef FLORENCE_COMPILER_HPP
#define FLORENCE_COMPILER_HPP
#include <queue>
#include <set>
#include <string>
#include "IRElements.hpp"
#include "Tokens.hpp"
#include "ErrorLogger.hpp"

namespace Florence::FSCompiler {
    /**
     * Represents the compiler that takes IRElements queue and
     *  outputs Assembly instructions.
     */
    class Compiler {
    private:
        std::queue<IRElements::IRElement*> intermediate_representation_;
        std::string logic_segment_; /** Contains the actual section jumping. */
        std::string data_segment_; /** Contains the data segment, strings. */
        std::set<std::string> labels_; /** Vector of already existing labels. */
        ErrorLogger *error_logger_; /** The error logger */

        /**
         * Compile the global elements.
         *
         * @param a GlobalIRElements element to compile to the data segment.
         */
        void compile_element(IRElements::GlobalsIRElement *element);

        /**
         * Compile a section element.
         *
         * @param Section element to compile.
         */
        void compile_element(IRElements::TextSectionIRElement *element);

        /**
         * Compile an image section element.
         *
         * @param an image section element.
         */
        void compile_element(IRElements::ImageSectionIRElement *element);

        /**
         * Compile a direct section element.
         *
         * @param a direct section element.
         */
        void compile_element(IRElements::DirectSectionIRElement *element);

        /**
         * Compile the text of a section to the data segment.
         *
         * @param label Name of the label owning the text.
         * @param text Text portion of the text.
         */
        void compile_element(IRElements::SectionIRElement *element);

        /**
         * Compile a single global token to the data segment.
         * @param token Token to compile.
         * @param add_suffix: If true, globals have a _text added to their name.
         * @param null_terminated: If true, global value strings are terminated with zero.
         */
        void compile_element(Tokens::GlobalToken *token, bool add_suffix, bool null_terminated);

        /**
         * Compile all sections.
         */
        void compile_sections();

        /**
         * Compile all of the globals.
         */
        void compile_data_segment();

        /**
         * Emit an error.
         *
         * @param error_type: Type of the error.
         * @param row: Row the error occurred in.
         * @param column: Column the error occurred in.
         * @param error_message: Message of the error.
         */
        void emit_error(FlorenceError::Error error_type, int row, int column, std::string error_message);

        /**
         * Dequeue an element from the Intermediate Representation
         *  queue.
         *
         * @return the dequeued Intermediate Representation element or nullptr if empty.
         */
        IRElements::IRElement *dequeue();

        /**
         * Peek to the front of the IR Queue.
         *
         * @return the element at the front or nullptr if empty.
         */
        IRElements::IRElement *peek();

    public:
        Compiler(std::queue<IRElements::IRElement *> elements, ErrorLogger *logger) : intermediate_representation_(elements), error_logger_(logger) {};
        ~Compiler();

        /**
         * Compile the Intermediate Representation to the
         *  x86 Assembly source code written for FASM Assembler.
         *
         * @return the generated assembler.
         */
        std::string compile();
    };
}

#endif //FLORENCE_COMPILER_HPP
