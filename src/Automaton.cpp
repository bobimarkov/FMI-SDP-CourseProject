#include "Automaton.hpp"
#include <stack>

#define EPSILON (char)238

void Automaton::copy(const Automaton& other) {
    if (this != &other) {
        states = other.states;
        beginningStates = other.beginningStates;
        finalStates = other.finalStates;

        transitions = other.transitions;
        neighbours = other.neighbours;
    }
}

Automaton::Automaton(std::vector<TransitionState> trans) {
    for (TransitionState transition : trans) {
        EdgeState edge = transition.first;
        TransitionLetters letters = transition.second;
        
        State state1 = edge.first;
        State state2 = edge.second;

        addTransition(std::make_pair(std::get<0>(state1), std::get<0>(state2)), letters);
        addState(std::get<0>(state1), std::get<1>(state1), std::get<2>(state1));
        addState(std::get<0>(state2), std::get<1>(state2), std::get<2>(state2));

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

    neighbours[state];
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

    neighbours[edge.first].insert(edge.second);
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

    std::cout << "\nTransitions: ";
    for (auto transition : transitions) {
        printf("From %d to %d | ", transition.first.first, transition.first.second);
        for (char letter : transition.second) {
            std::cout << letter << " ";
        }
        std::cout << std::endl;
    }
}

bool Automaton::recognize(const std::string& str) const {
    for (int beginningState: beginningStates) {
        for (int finalState: finalStates) {
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
                }

                path.push_back(top.second);

                for (int vert : neighbours.at(top.second)) { 
                        dfsStack.push({top.second, vert});
                    }
                
            }


        }
    }
    return false;
}

Automaton Automaton::aUnion(const Automaton& other) {
    Automaton newAutomaton = *this;
    
    int firstLastState = *(--newAutomaton.getStates().end());

    for(int secondState : other.getStates()) {
        newAutomaton.addState(secondState + firstLastState);
        if (other.beginningStates.count(secondState) != 0) newAutomaton.addBeginningState(secondState + firstLastState);
        if (other.finalStates.count(secondState) != 0) newAutomaton.addFinalState(secondState + firstLastState);
    }

    for (auto it : other.getTransitions()) {
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

Automaton Automaton::aConcat(const Automaton& other) {
    Automaton newAutomaton;
    newAutomaton.setStates(this -> getStates());
    newAutomaton.setBeginningStates(this -> getBeginningStates());
    newAutomaton.setTransitions(this -> getTransitions());

    int firstLastState = *(--newAutomaton.getStates().end());

    for(int secondState : other.getStates()) {
        newAutomaton.addState(secondState + firstLastState);
        if (other.getFinalStates().count(secondState) != 0) newAutomaton.addFinalState(secondState + firstLastState);
    }

    for(auto transition : other.getTransitions()) {
        newAutomaton.addTransition(std::make_pair(transition.first.first + firstLastState, transition.first.second + firstLastState), transition.second);   
    }

    for(int firstEndingState : this -> getFinalStates()) {
        for(int secondBeginningState : other.getBeginningStates()) {
            newAutomaton.addTransition(std::make_pair(firstEndingState, secondBeginningState + firstLastState), {EPSILON});
        }   
    }

    return newAutomaton;
}

Automaton Automaton::aComplement(const Automaton& other) {

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

void Automaton::determinite() {
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