#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP
#include <string>

class GraphExceptions {
public:
    std::string what() {return this->errMessg;}
    GraphExceptions(const std::string& error) {this->errMessg = error;}
private:
    std::string errMessg;
};

#endif