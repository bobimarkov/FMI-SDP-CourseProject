#ifndef __REGEX_HPP_
#define __REGEX_HPP_

#include <iostream>
#include "Tokenizer.hpp"
#include "Automaton.hpp"

namespace RegexUtils {
    ///Converts a regular expression to reversed polish notation regular expression.
    std::string shuntingYardAlgo(Tokenizer);

    ///Evaluates a RPN regular expression.
    Automaton evaluateRegex (Tokenizer);

    ///Converts the given automaton to regular expression.
    std::string automatonToRegex(Automaton, int, int, int);
}


#endif