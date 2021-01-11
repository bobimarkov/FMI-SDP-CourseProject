#ifndef __TOKENIZER_HPP_
#define __TOKENIZER_HPP_

#include <iostream>

class Tokenizer {
private:
    ///The input stream which the tokenizer manages.
    std::istream& in;    

    ///Clears the whitespaces from the stream.
    void clearWhitespace();

public:
    struct Token {
        enum Type {open_bracket, closing_bracket, oper, letter, error};
        Type type;
        char symbol;
    };

    Tokenizer(std::istream&);

    ///Checks if the tokenizer has any tokens left to managing.
    bool hasMore() const;

    ///Gets the next token.
    Token getToken();
};

#endif