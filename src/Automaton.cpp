#include "Automaton.hpp"
#include <stack>
#include <sstream>
#include "RegexUtils.hpp"

void Automaton::copy(const Automaton& other) {
    if (this != &other) {
        states = other.states;
        beginningStates = other.beginningStates;
        finalStates = other.finalStates;

        transitions = other.transitions;
        neighbours = other.neighbours;
    }
}

void Automaton::updateNeighbours() {
    this -> neighbours.clear();

    for (int state : states) {
        neighbours[state];
    }

    for (auto t : transitions) {
        neighbours.at(t.first.first).insert(t.first.second);
    }
}

Automaton::Automaton(const Automaton& other) {
    copy(other);
}


std::set<int> Automaton::getStates() const {
    return this -> states;
}

std::set<int> Automaton::getBeginningStates() const {
    return this -> beginningStates;
}

std::set<int> Automaton::getFinalStates() const {
    return this -> finalStates;
}

Automaton::Transitions Automaton::getTransitions() const {
    return this -> transitions;
}

std::map<int, std::set<int>> Automaton::getNeighbours() const {
    return this -> neighbours;
}

void Automaton::setStates(std::set<int> other) {
    this -> states = other;
}

void Automaton::setBeginningStates(std::set<int> other) {
    this -> beginningStates = other;
}

void Automaton::setFinalStates(std::set<int> other) {
    this -> finalStates = other;
}

void Automaton::setTransitions(Transitions other) {
    this -> transitions = other;
}

void Automaton::addState(const int state, bool beginning = false, bool final = false) {
    states.insert(state);
    if (beginning) {
        beginningStates.insert(state);
    }
    if (final) {
        finalStates.insert(state);
    }

    updateNeighbours();
}

void Automaton::addBeginningState(const int state) {
    addState(state, true);
}

void Automaton::addFinalState(const int state) {
    addState(state, false, true);
}

void Automaton::addTransition(Edge edge, TransitionLetters letters) {
    for (char c : letters) {
        transitions[edge].insert(c);
    }

    addState(edge.first);
    addState(edge.second);

    updateNeighbours();
}

void Automaton::printInfo() const {
    std::cout << "States: ";
    for (int state : states) {
        std::cout << state << " ";
    }

    std::cout << "\nBeginning states: ";
    for (int beginningState : beginningStates) {
        std::cout << beginningState << " ";
    }

    std::cout << "\nFinal states: ";
    for (int finalState : finalStates) {
        std::cout << finalState << " ";
    }

    std::cout << "\nTransitions:\n";
    for (auto transition : transitions) {
        printf("From %d to %d -> ", transition.first.first, transition.first.second);
        for (char letter : transition.second) {
            std::cout << letter << " ";
        }
        std::cout << std::endl;
    }
}

bool Automaton::recognize(const std::string& str) const {
    Automaton automatonToWorkWith = *this;
    automatonToWorkWith.determine();
    for (int beginningState: automatonToWorkWith.beginningStates) {
            std::string currentWord = ""; 
            std::vector<int> path;

            std::stack<Edge> dfsStack;

            int current = beginningState;

            dfsStack.push({current, current});

            while(!dfsStack.empty())
            {
                Edge top = dfsStack.top();
                dfsStack.pop();

                while(!path.empty() && path[path.size()-1] != top.first) {
                    path.pop_back();
                    if (automatonToWorkWith.transitions.count(top) > 0 && automatonToWorkWith.transitions.at(top).count(EPSILON) == 0) {
                        currentWord.pop_back();
                    }
                }

                path.push_back(top.second);
                if (automatonToWorkWith.transitions.count(top) > 0 && automatonToWorkWith.transitions.at(top).count(EPSILON) == 0) {
                    currentWord.push_back(str[currentWord.size()]);
                }

                if (currentWord == str && automatonToWorkWith.finalStates.count(top.second) > 0) return true;

                for (int vert : automatonToWorkWith.neighbours.at(top.second)) { 
                    if (automatonToWorkWith.transitions.at(std::make_pair(top.second, vert)).count(str[currentWord.size()]) > 0 
                        || automatonToWorkWith.transitions.at(std::make_pair(top.second, vert)).count(EPSILON) > 0 
                        || automatonToWorkWith.transitions.at(std::make_pair(top.second, vert)).count('?') > 0) {

                        dfsStack.push(std::make_pair(top.second, vert));
                    
                    }
                }
            }
    }
    return false;
}

Automaton Automaton::un(const Automaton& first, const Automaton& second) {
    Automaton newAutomaton = first;
    
    int firstLastState = *(--newAutomaton.getStates().end());

    for(int secondState : second.getStates()) {
        newAutomaton.addState(secondState + firstLastState);
        if (second.beginningStates.count(secondState) != 0) newAutomaton.addBeginningState(secondState + firstLastState);
        if (second.finalStates.count(secondState) != 0) newAutomaton.addFinalState(secondState + firstLastState);
    }

    for (auto it : second.getTransitions()) {
        newAutomaton.addTransition(std::make_pair(it.first.first + firstLastState, it.first.second + firstLastState), it.second);
    }

    int newState = *(--newAutomaton.getStates().end()) + 1;

    newAutomaton.addState(newState);

    for(int begState : newAutomaton.getBeginningStates()) {
        newAutomaton.addTransition(std::make_pair(newState, begState), {EPSILON});
    }

    newAutomaton.setBeginningStates(std::set<int>());
    newAutomaton.addBeginningState(newState);

    return newAutomaton;
}

Automaton::TransitionLetters Automaton::getCommonLetters (const TransitionLetters first, const TransitionLetters second) {
    TransitionLetters common;
    for (char firstTransition : first) {
        if (second.count(firstTransition) > 0) common.insert(firstTransition);
    }
    return common;
}

Automaton Automaton::intersection(const Automaton& first, const Automaton& second) {
    std::set<std::pair<int, int>> newStates;
    for (int first : first.states) {
        for (int second : second.states) {
            newStates.insert(std::make_pair(first,second));
        }
    }

    Automaton newAutomaton;
    int stateN1 = 0, stateN2 = 0; 
    for (std::pair<int, int> firstState : newStates) {
        stateN1++;
        if (first.beginningStates.count(firstState.first) > 0 && second.beginningStates.count(firstState.second) > 0) {
            newAutomaton.addBeginningState(stateN1);
        }
        if (first.finalStates.count(firstState.first) > 0 && second.finalStates.count(firstState.second) > 0) {
            newAutomaton.addFinalState(stateN1);
        }
        for (std::pair<int, int> secondState : newStates ) {
            stateN2++;
            if (firstState != secondState) {
                TransitionLetters commonLetters = getCommonLetters(first.transitions.at(std::make_pair(firstState.first, secondState.first)), second.transitions.at(std::make_pair(firstState.second, secondState.second)));
                if (commonLetters.size() > 0) {
                    newAutomaton.addTransition(std::make_pair(stateN1, stateN2), commonLetters);
                }
            }
        }
        stateN2 = 0;
    }
    
    return newAutomaton;
}

Automaton Automaton::concat(const Automaton& first, const Automaton& second) {
    Automaton newAutomaton;
    newAutomaton.setStates(first.getStates());
    newAutomaton.setBeginningStates(first.getBeginningStates());
    newAutomaton.setTransitions(first.getTransitions());

    int firstLastState = *(--newAutomaton.getStates().end());

    for(int secondState : second.getStates()) {
        newAutomaton.addState(secondState + firstLastState);
        if (second.getFinalStates().count(secondState) != 0) newAutomaton.addFinalState(secondState + firstLastState);
    }

    for(auto transition : second.getTransitions()) {
        newAutomaton.addTransition(std::make_pair(transition.first.first + firstLastState, transition.first.second + firstLastState), transition.second);   
    }

    for(int firstEndingState : first.getFinalStates()) {
        for(int secondBeginningState : second.getBeginningStates()) {
            newAutomaton.addTransition(std::make_pair(firstEndingState, secondBeginningState + firstLastState), {EPSILON});
        }   
    }

    return newAutomaton;
}

Automaton Automaton::complement(const Automaton& automaton) {
    Automaton newAutomaton = automaton;
    std::set<int> newFinalStates;

    for (int i : automaton.states) {
        if (automaton.finalStates.count(i) == 0) {
            newFinalStates.insert(i);
        }
    }

    newAutomaton.setFinalStates(newFinalStates);
    return newAutomaton;
}

Automaton Automaton::iteration(const Automaton& automaton) {
    Automaton newAutomaton = automaton;

    for (int finalState : newAutomaton.finalStates) {
        for (int beginningState : newAutomaton.beginningStates) {
            if (newAutomaton.neighbours.at(finalState).count(beginningState) == 0) {
                newAutomaton.addTransition(std::make_pair(finalState, beginningState), {EPSILON});
            }
            if (newAutomaton.finalStates.count(beginningState) == 0) {
                newAutomaton.addFinalState(beginningState);
            }
        }
    }

    return newAutomaton;
}

std::set<char> Automaton::getAlphabet() const {
    std::set<char> alphabet;
    for (auto transition : transitions) {
        for (char letter : transition.second) {
            if (letter != EPSILON) {
                alphabet.insert(letter);
            }
        }
    }
    return alphabet;
}

bool Automaton::equalSets(std::set<int> s1, std::set<int> s2) {
    if (s1.size() == s2.size()) {
        for (int x : s1) {
            if (s2.count(x) == 0) return false;
        }
        return true; 
    }
    else return false;
}

bool Automaton::containsInMainColumn(std::vector<std::vector<std::set<int>>> grid, std::set<int> state) {
    for (int i = 0; i < grid.size(); i++) {
        if (equalSets(grid[i][0], state)) return true;       
    }
    return false;
}

void Automaton::determine() {
    std::set<char> alphabet = getAlphabet();
    std::vector<std::vector<std::set<int>>> grid;

    std::set<int> beginningState;
    for (int begState : beginningStates) beginningState.insert(begState);

    for(std::set<int>::iterator begState = beginningState.begin(); begState != beginningState.end(); begState++) {
        for(auto transition : transitions) {
            if (transition.second.count(EPSILON) > 0 && transition.first.first == *begState && beginningState.count(transition.first.second) == 0) {
                beginningState.insert(transition.first.second);
                begState = beginningState.begin();
            }
        }
    }

    grid.push_back(std::vector<std::set<int>>(1 + alphabet.size()));
    grid[0][0] = beginningState;
    for (int i = 0; i < grid.size(); i++) {
        for(int j = 1; j < grid[i].size(); j++) {
            std::set<int> newState;
            for (int state : grid[i][0]) {
                for (auto transition : transitions) {
                    if (transition.first.first == state && transition.second.count(*std::next(alphabet.begin(), j-1)) > 0) newState.insert(transition.first.second);
                }
            }

            for(std::set<int>::iterator state = newState.begin(); state != newState.end(); state++) {
                for(auto transition : transitions) {
                    if (transition.second.count(EPSILON) > 0 && transition.first.first == *state && newState.count(transition.first.second) == 0) {
                        newState.insert(transition.first.second);
                        state = newState.begin();
                    }
                }
            }

            grid[i][j] = newState;
            if (!containsInMainColumn(grid, newState) && newState.size() > 0) {
                grid.push_back(std::vector<std::set<int>>(1 + alphabet.size()));
                grid.back()[0] = newState;
            } 
        }
    }

    std::vector<std::vector<int>> simpleGrid(grid.size(), std::vector<int>(grid[0].size()));
    for(int i = 0; i < simpleGrid.size(); i++) simpleGrid[i][0] = i+1;

    for (int i = 0; i < grid.size(); i++) {
        for(int j = 1; j < grid[i].size(); j++) {
            for(int n = 0; n < grid.size(); n++) {
                if (equalSets(grid[n][0], grid[i][j])) simpleGrid[i][j] = n + 1;
            }
        }
    }

    Automaton newAutomaton;
    for(int i = 0; i < simpleGrid.size(); i++) newAutomaton.addState(i+1);
    newAutomaton.addBeginningState(1);

    for (int i = 0; i < simpleGrid.size(); i++) {
        for (int j = 1; j < simpleGrid[i].size(); j++) {
            if (simpleGrid[i][j] > 0) newAutomaton.addTransition(std::make_pair(simpleGrid[i][0], simpleGrid[i][j]), {*std::next(alphabet.begin(), j - 1),});
        }
    }

    for (int i = 0; i < grid.size(); i++) {
        for(int endingState : finalStates) {
            if (grid[i][0].count(endingState) != 0) newAutomaton.addFinalState(i+1);
        }
    }

    *this = newAutomaton;
}

void Automaton::readRegex(std::string regex) {
    std::stringstream toShuntingYard;
    toShuntingYard << regex;
    Tokenizer t1(toShuntingYard);

    std::stringstream toEval;
    toEval << RegexUtils::shuntingYardAlgo(t1);
    Tokenizer t2(toEval);

    *this = RegexUtils::evaluateRegex(t2);
}

std::string Automaton::convertToRegex() const {
    Automaton newAutomaton = *this;
    newAutomaton.determine();

    std::stringstream result;

    std::set<int> fStates = newAutomaton.finalStates;
    result << RegexUtils::automatonToRegex(newAutomaton, *newAutomaton.beginningStates.begin(), *fStates.begin(), newAutomaton.states.size() + 1);
    fStates.erase(fStates.begin());

    for (int finalState : fStates) {
        result << "+" << RegexUtils::automatonToRegex(newAutomaton, *newAutomaton.beginningStates.begin(), finalState, newAutomaton.states.size() + 1);
    }

    return result.str();
}


Automaton& operator >> (Automaton& automaton, std::string& str) {
    str = automaton.convertToRegex();
    return automaton;
}

Automaton& operator << (Automaton& automaton, std::string& str) {
    automaton.readRegex(str);
    return automaton;
}

Automaton& operator >> (std::string& str, Automaton& automaton) {
    str = automaton.convertToRegex();
    return automaton;
}

Automaton& operator << (std::string& str, Automaton& automaton) {
    automaton.readRegex(str);
    return automaton;
}