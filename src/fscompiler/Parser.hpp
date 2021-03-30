#ifndef FLORENCE_PARSER_HPP
#define FLORENCE_PARSER_HPP
#include <queue>
#include <utility>
#include "IRElements.hpp"
#include "ErrorLogger.hpp"

namespace Florence::FSCompiler {
    /**
     * Class that acts as a Parser. The parser,
     *  converts tokens to intermediate representation.
     */
    class Parser {
    private:
        std::queue<IRElements::IRElement*> intermediate_representation_; // An intermediate representation.
        std::queue<Tokens::Token*> token_queue_; // A queue of tokens.
        ErrorLogger *error_logger_; // A logger.

        /**
 * Group the globals into a globals
 */
        void parse_globals();

        /**
         * Parse every section.
         */
        void parse_sections();

        /**
         * Parse a single section.
         */
        void parse_section();

        /**
         * Parse an image section.
         */
        void parse_image_section();

        /**
         * Emit an error.
         *
         * @param error: Type of the error.
         * @param row: Row error occurs in.
         * @param col: Column the error occurs in.
         * @param error_message: Error message.
         */
        void emit_error(FlorenceError::Error error, int row, int col, std::string error_message);

        /**
         * Dequeue a token.
         * @return The token just dequeued.
         */
        Tokens::Token *dequeue();

        /**
         * Return the token in the back
         *  without dequeuing.
         * @return The token at the back.
         */
        Tokens::Token *peek();

    public:
        /**
         * Parse the token queue and turn it into an intermediate
         *  representation.
         *
         * @return A queue of IRElement objects.
         */
        std::queue<IRElements::IRElement*> parse();

        Parser(std::queue<Tokens::Token*> token_queue, ErrorLogger *logger) : token_queue_(token_queue), error_logger_(logger) {};
        ~Parser();
    };
};

#endif //FLORENCE_PARSER_HPP
