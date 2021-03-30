#ifndef FLORENCE_IRELEMENTS_HPP
#define FLORENCE_IRELEMENTS_HPP
#include "Tokens.hpp"
#include <queue>

namespace Florence::FSCompiler::IRElements {
    enum IRElementType {
        GLOBALS_IR_ELEMENT, // Global IR Element.
        SECTION_IR_ELEMENT, // A section with choices.
        IMAGE_SECTION_IR_ELEMENT, // A section that shows an image.
        DIRECT_SECTION_IR_ELEMENT // Direct jump.
    };

    /**
     * An Intermediate Representation of
     *  the element.
     */
    struct IRElement{
        IRElementType element_type; // Type of the element.
    };

    /**
     * Represents a group of globals
     *  these are declared at the top of a
     *  FlorenceScript element.
     */
    struct GlobalsIRElement : IRElement {
        std::queue<Tokens::GlobalToken*> globals; // Global tokens.
    };

    /**
     * Representation of a section in the game.
     */
    struct SectionIRElement : IRElement {
        Tokens::LabelToken *section_label; // The label of a section.
        std::string text;
        std::queue<Tokens::ChoiceToken*> choices; // The choices of the section.
    };

    /**
     * A section that shows an image, waits for a keypress
     *  and later progresses.
     */
    struct ImageSectionIRElement : IRElement {
        Tokens::DirectToken *direct; // Section to jump to after showing image.
        Tokens::LabelToken *section_label; // The label of a section.
    };

    /**
     * A section that prints some text and then immediately progresses.
     */
    struct DirectSectionIRElement : IRElement {
        Tokens::DirectToken *direct; // Section to jump to directly.
        std::string text;
        Tokens::LabelToken *section_label; // Label of a section.
    };
}

#endif //FLORENCE_IRELEMENTS_HPP
