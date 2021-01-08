#ifndef __AUTOMATON_HPP_
#define __AUTOMATON_HPP_

#include <iostream>
#include <set>
#include <map>
#include <vector>
#include <tuple>

class Automaton {
private:
    using Edge = std::pair<int, int>;
    using TransitionLetters = std::set<char>;
    using Transitions = std::map<Edge, TransitionLetters>;

    using State = std::tuple<int, bool, bool>;
    using EdgeState = std::pair<State, State>;
    using TransitionState = std::pair<EdgeState, TransitionLetters>;

    std::set<int> states;
    std::set<int> beginningStates;
    std::set<int> finalStates;
    
    Transitions transitions;
    std::map<int, std::set<int>> neighbours; 

    void copy(const Automaton&);

    static bool equalSets(std::set<int>, std::set<int>);
    static bool containsInMainColumn(std::vector<std::vector<std::set<int>>>, std::set<int>);
    std::set<char> getAlphabet() const;

public:
    Automaton() = default;
    Automaton(std::vector<TransitionState>);
    Automaton(const Automaton&); 

    std::set<int> getStates() const;
    std::set<int> getBeginningStates() const;
    std::set<int> getFinalStates() const;
    Transitions getTransitions() const;

    void setStates(std::set<int>);
    void setBeginningStates(std::set<int>);
    void setFinalStates(std::set<int>);
    void setTransitions(Transitions);

    void addState(const int, bool, bool);
    void addBeginningState(const int);
    void addFinalState(const int);
    void addTransition(Edge, TransitionLetters);

    void printInfo() const;
    bool recognize(const std::string&) const; //
    Automaton aUnion(const Automaton&);
    Automaton aIntersection(const Automaton&); //
    Automaton aConcat(const Automaton&); 
    Automaton aComplement(const Automaton&); //

    void determinite();


};

#endif

/*
TODO:
    проверка за принадлежност на дума към езика на автомата
  ? потокови операции, които позволяват автоматите да се разглеждат като консуматори на низове
    обединение на два автомата - done
    сечение на два автомата
    конкатенация на два автомата - done
    итерация на автомат
    допълнение на автомат
    извеждане на информация за автомата - done
    операция за преобразуване на регулярен израз до автомат
    операция за преобразуване на автомат до низ, съдържащ регулярен израз (теорема на Клини)

    детерминизация на автомат - done
    минимизация на автомат
    да се реализират операции, които проверяват дали езикът на даден автомат е:
        празен (не се разпознава нито една дума)
        краен (разпознават се краен брой думи)
        пълен (разпознават се всички думи)

*/