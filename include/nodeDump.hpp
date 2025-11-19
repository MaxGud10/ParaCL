#include <string>

const std::string SET_NODE              = "node";
const std::string SET_MRECORD_SHAPE     = "[shape=Mrecord; ";
const std::string SET_LABEL             = "label = \"{ ";
const std::string SET_ADR               = "| adr =" ;
const std::string END_LABEL             = "}\";";
const std::string SET_FILLED            = "style=filled; ";
const std::string SET_COLOR             = "fillcolor=\"#";
const std::string END_NODE              = "\"];\n";
const std::string SET_LINK              = "->";


std::string generateFileName(const std::string& prefix, const std::string& extension);

enum NodeColors {
    VARIABLE_NODE_COLOR     = 0xC2EEC7,
    BINARYOP_NODE_COLOR     = 0xF2E3C6,
    UNARYOP_NODE_COLOR      = 0xD3ECE1,
    ASSIGN_NODE_COLOR       = 0xEED7A1,
    WHILE_NODE_COLOR        = 0xCD8B62,
    FOR_NODE_COLOR          = 0xF7EFD2,
    IF_NODE_COLOR           = 0xFFD09B,
    PRINT_NODE_COLOR        = 0xFFB0B0,
    IN_NODE_COLOR           = 0xAFEEEE,
    CONSTANT_NODE_COLOR     = 0x98FB98
};
