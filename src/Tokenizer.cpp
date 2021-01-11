#include "Tokenizer.hpp"

#include <iostream>

Tokenizer::Tokenizer(std::istream& in):in(in) {

}

bool Tokenizer::hasMore() const {
    return in.good();
}

void Tokenizer::clearWhitespace() {
    while (in && std::isspace(in.peek())) {
        in.get();
    }
}

Tokenizer::Token Tokenizer::getToken() {
    clearWhitespace();

    Token token = {Token::error, 0};
    char c = in.peek();

    switch (c) {
        case '(': 
            token.type = Token::open_bracket;
            token.symbol = in.get();
            break;
        case ')': 
            token.type = Token::closing_bracket;
            token.symbol = in.get();
            break;
        case '+':
        case '.':
        case '*':
        case '&':
            token.type = Token::oper;
            token.symbol = in.get();
            break;
        default:
            token.type = Token::letter;
            token.symbol = in.get();
            break;
    } 

    return token;
}

