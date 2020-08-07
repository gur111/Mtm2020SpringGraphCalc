#include "Exceptions.h"

namespace GraphCalc {
// ==== GraphCalcError ====
GraphCalcError::GraphCalcError() : msg("Error: ") {}
const char *GraphCalcError::what() const noexcept { return msg.c_str(); }

// ==== Syntax Error ====
SyntaxError::SyntaxError(std::string info) { msg += "Invalid syntax. " + info; }

// ==== Invalid Format Error ====
InvalidFormat::InvalidFormat(std::string info) {
    msg += "Invalid format. " + info;
}

// ==== Multiple Declarations Error ====
MultipleDeclarations::MultipleDeclarations(std::string info) {
    msg += "Multiple Declarations. " + info;
}

// ==== Missing Declaration Error ====
Missing::Missing(std::string info) { msg += "Missing Declaration. " + info; }

// ==== Unknown Error ====
Unknown::Unknown(std::string info) { msg += "Unknown. " + info; }

}  // namespace GraphCalc
