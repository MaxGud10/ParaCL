#pragma once

#include <string_view>

inline constexpr std::string_view SET_NODE          = "node";
inline constexpr std::string_view SET_MRECORD_SHAPE = "[shape=Mrecord; ";
inline constexpr std::string_view SET_LABEL         = "label = \"{ ";
inline constexpr std::string_view SET_ADR           = "| adr =";
inline constexpr std::string_view END_LABEL         = "}\";";
inline constexpr std::string_view SET_FILLED        = "style=filled; ";
inline constexpr std::string_view SET_COLOR         = "fillcolor=\"#";
inline constexpr std::string_view END_NODE          = "\"];\n";
inline constexpr std::string_view SET_LINK          = "->";

std::string generateFileName(std::string_view prefix, std::string_view extension);

namespace AST::dump_style
{
    inline constexpr int VARIABLE_NODE_COLOR = 0xC2EEC7;
    inline constexpr int BINARYOP_NODE_COLOR = 0xF2E3C6;
    inline constexpr int UNARYOP_NODE_COLOR  = 0xD3ECE1;
    inline constexpr int ASSIGN_NODE_COLOR   = 0xEED7A1;
    inline constexpr int WHILE_NODE_COLOR    = 0xCD8B62;
    inline constexpr int FOR_NODE_COLOR      = 0xF7EFD2;
    inline constexpr int IF_NODE_COLOR       = 0xFFD09B;
    inline constexpr int PRINT_NODE_COLOR    = 0xFFB0B0;
    inline constexpr int IN_NODE_COLOR       = 0xAFEEEE;
    inline constexpr int CONSTANT_NODE_COLOR = 0x98FB98;
}


