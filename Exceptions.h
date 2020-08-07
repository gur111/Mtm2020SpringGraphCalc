#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H
#include <exception>
#include <string>

namespace GraphCalc {

class GraphCalcError : public std::exception {
   protected:
    std::string msg;

   public:
    GraphCalcError();
    const char *what() const noexcept override;
};

class SyntaxError : public GraphCalcError {
   public:
    SyntaxError(std::string info);
};

class InvalidFormat : public GraphCalcError {
   public:
    InvalidFormat(std::string info);
};
class MultipleDeclarations : public GraphCalcError {
   public:
    MultipleDeclarations(std::string info);
};
class Missing : public GraphCalcError {
   public:
    Missing(std::string info);
};
class Unknown : public GraphCalcError {
   public:
    Unknown(std::string info);
};
}  // namespace GraphCalc

#endif