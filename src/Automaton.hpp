#ifndef __AUTOMATON_HPP_
#define __AUTOMATON_HPP_

#include <iostream>
#include <set>
#include <map>
#include <vector>

#define EPSILON (char)238

class Automaton {
public: 
    ///Edge of the automaton
    using Edge = std::pair<int, int>;

    ///Set of the transition's letters
    using TransitionLetters = std::set<char>;

    /** Map of edge and the transition letters
     * 
     *  Represents all transitions in the automaton.
     * 
     */
    using Transitions = std::map<Edge, TransitionLetters>;

private:
    ///All states.
    std::set<int> states;

    ///All beginning states.
    std::set<int> beginningStates;

    ///All final states
    std::set<int> finalStates;
    
    ///All transitions
    Transitions transitions;

    /** Map of state and its neighbours. 
     * 
     *  Contains a state and a set of all states that can be directly accessed by the current state.
     *  
     * 
     */
    std::map<int, std::set<int>> neighbours; 

    ///Copies another automaton
    void copy(const Automaton&);

    ///Updates the neighbours map
    void updateNeighbours();

    ///Checks if the given 2 sets are equal 
    static bool equalSets(std::set<int>, std::set<int>);

    /** Checks if the main column contains the given set
     * 
     * 
     * 
     */
    static bool containsInMainColumn(std::vector<std::vector<std::set<int>>>, std::set<int>);
    
    ///Gets automaton's alphabet
    std::set<char> getAlphabet() const;

    ///Gets the common letters in the both sets 
    static TransitionLetters getCommonLetters (const TransitionLetters, const TransitionLetters);

public:
    Automaton() = default;
    Automaton(const Automaton&); 

    std::set<int> getStates() const;
    std::set<int> getBeginningStates() const;
    std::set<int> getFinalStates() const;
    Transitions getTransitions() const;
    std::map<int, std::set<int>> getNeighbours() const;

    void setStates(const std::set<int>);
    void setBeginningStates(const std::set<int>);
    void setFinalStates(const std:: set<int>);
    void setTransitions(const Transitions);

    void addState(const int, bool, bool);
    void addBeginningState(const int);
    void addFinalState(const int);
    void addTransition(Edge, TransitionLetters);

    /** Prints automaton's information.
     * 
     * All states, beginning and final states, transitions.
     * 
     */
    void printInfo() const;

    ///Checks if the automaton recognizes the given word 
    bool recognize(const std::string&) const; 

    ///The union of 2 automatons
    static Automaton un(const Automaton&, const Automaton&);

    ///The interesection of 2 automatons
    static Automaton intersection(const Automaton&, const Automaton&); 

    ///The concatenation of 2 automatons
    static Automaton concat(const Automaton&, const Automaton&); 

    ///The complement of an automaton
    static Automaton complement(const Automaton&);  

    ///The iteration of an automaton
    static Automaton iteration(const Automaton&);

    ///Converts the given regular expression into automaton.
    void readRegex(const std::string);

    ///Converts the current automaton in a regular expression.
    std::string convertToRegex() const;

    ///Determines an automaton
    void determine();

    ///Stream operator that calls the convertToRegex() function
    friend Automaton& operator >> (Automaton&, std::string&);

    ///Stream operator that reads regular expression and passes it to the automaton.
    friend Automaton& operator << (Automaton&, std::string&);

    ///Stream operator that reads regular expression and passes it to the automaton.
    friend Automaton& operator >> (std::string&, Automaton&);

    ///Stream operator that calls the convertToRegex() function
    friend Automaton& operator << (std::string&, Automaton&);

};

#endif