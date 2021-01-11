#include "RegexUtils.hpp"

#include <sstream>
#include <stack>

int getPriority(Tokenizer::Token token) {
    switch (token.symbol) {
        case '*': return 3;
        case '.': return 2;
        case '&': 
        case '+': return 1;
        default: return 0;
    }
}

Automaton createBasicAutomaton (Automaton::TransitionLetters letters) {
    Automaton newAutomaton;
    newAutomaton.addState(1, true, false);
    newAutomaton.addState(2, false, true);
    newAutomaton.addTransition(std::make_pair(1,2), letters);

    return newAutomaton;
}

Automaton applyKleeneStar (Automaton automaton) {
    Automaton newAutomaton = automaton;
    return Automaton::iteration(automaton);
}

Automaton applyRestOperations (char oper, Automaton first, Automaton second) {
    switch (oper) {
        case '.':
            return Automaton::concat(second, first);
        case '+':
            return Automaton::un(second, first);
        case '&': 
            return Automaton::intersection(second, first);
    }
    return first;
}

Automaton RegexUtils::evaluateRegex (Tokenizer tokenizer) {
    std::stack<Automaton> automatonStack;

    Tokenizer::Token token = tokenizer.getToken();
    
    while (tokenizer.hasMore())
    {
        if (token.type == Tokenizer::Token::letter){
            char c = token.symbol;
            if (token.symbol == '@') c = EPSILON;

            automatonStack.push (createBasicAutomaton({c}));
        }
        else {
            if (token.type == Tokenizer::Token::oper) {
                if (automatonStack.size() >= 2) {
                    if (token.symbol == '*') {
                        Automaton automaton = automatonStack.top();
                        automatonStack.pop();

                        automatonStack.push(applyKleeneStar(automaton));
                    }   
                    else {
                        Automaton first = automatonStack.top(); 
                        automatonStack.pop();

                        Automaton second = automatonStack.top();
                        automatonStack.pop();

                        automatonStack.push ( applyRestOperations(token.symbol, first, second));
                    }
                }
                else if (automatonStack.size() == 1 && token.symbol == '*') {
                    Automaton automaton = automatonStack.top();
                    automatonStack.pop();

                    automatonStack.push(applyKleeneStar(automaton));
                }
            }
        }
        token = tokenizer.getToken();

    }
    return automatonStack.top();
}

std::string RegexUtils::shuntingYardAlgo (Tokenizer tokenizer) {
    std::stringstream sstr;

    std::stack<Tokenizer::Token> operatorStack;

    Tokenizer::Token token = tokenizer.getToken();

    while (tokenizer.hasMore()) { 
        if (token.type == Tokenizer::Token::letter) {
            sstr << token.symbol;
        }
        else if (token.type == Tokenizer::Token::oper) {
            while (!operatorStack.empty() && operatorStack.top().type == Tokenizer::Token::oper && getPriority(operatorStack.top()) >= getPriority(token)) {
                sstr << operatorStack.top().symbol;
                operatorStack.pop();
            }
            operatorStack.push(token);
        }
        else if (token.type == Tokenizer::Token::open_bracket) {
            operatorStack.push(token);
        }
        else if (token.type == Tokenizer::Token::closing_bracket) {
            while (!operatorStack.empty() && operatorStack.top().type != Tokenizer::Token::open_bracket) {
                sstr << operatorStack.top().symbol;
                operatorStack.pop();
            }
            operatorStack.pop();
        }
        token = tokenizer.getToken();
    }

    while (!operatorStack.empty()) {
        sstr << operatorStack.top().symbol;
        operatorStack.pop();
    }

    return sstr.str();
}

std::string RegexUtils::automatonToRegex(Automaton automaton, int state1, int state2, int k) {
    std::stringstream result;
    if (k == 1) {
        if (automaton.getNeighbours().at(state1).count(state2) == 0) {
            return "";
        }
        Automaton::TransitionLetters transitionLetters = automaton.getTransitions().at(std::make_pair(state1, state2));
        if (state1 == state2) {
            result << '@';
            for (char c : transitionLetters) {
                result << '+' << c;
            }
        }
        else {
            result << *transitionLetters.begin();
            transitionLetters.erase(transitionLetters.begin());
            for (char c : transitionLetters) {
                result << '+' << c;
            }
        }
    }
    else {
        bool includePlus = true;
        std::string firstExp = automatonToRegex(automaton, state1, state2, k-1);
        std::string secondExp = automatonToRegex(automaton, state1, k-1, k-1);
        std::string thirdExp = automatonToRegex(automaton, k-1, k-1, k-1);
        std::string fourthExp = automatonToRegex(automaton, k-1, state2, k-1);

        std::string kleeneStarExp = "(" + thirdExp + ")*.";
        if (thirdExp == "") kleeneStarExp = "";    

        std::string secondpart = "("+secondExp + ")." + kleeneStarExp + "(" + fourthExp + ")";
        if (firstExp == "") {
            includePlus = false;
        }
        else {
            firstExp = "(" + firstExp + ")";
        }
        if (secondExp == "" || fourthExp == "") {
            includePlus = false;
            secondpart = "";
        }
        result << firstExp;
        if (includePlus) {
            result << '+';
        }
        result << secondpart;
    }   
    return result.str();
}

