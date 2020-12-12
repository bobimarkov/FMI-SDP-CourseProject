#ifndef __AUTOMATON_HPP_
#define __AUTOMATON_HPP_

#include <iostream>
#include <unordered_set>

class Automaton {
public:
    struct Transition;
    struct State;

    struct State {
        bool beginning;
        bool final;
        unsigned int id;
        std::unordered_set<Transition> transitions;

        State();
        State(bool, bool, std::unordered_set<Transition>);
    };

    struct Transition {
        char letter;
        State* to;

        Transition() = default;
        Transition(char, State*);
    };
    
    std::unordered_set<State*> begStates;
    unsigned int size;
public:
    Automaton() = default;
    Automaton(std::unordered_set<State*>);
    
    bool recognize(const std::string&) const;



};

#endif

/*
TODO:
    проверка за принадлежност на дума към езика на автомата
    потокови операции, които позволяват автоматите да се разглеждат като консуматори на низове
    обединение на два автомата
    сечение на два автомата
    конкатенация на два автомата
    итерация на автомат
    допълнение на автомат
    извеждане на информация за автомата
    операция за преобразуване на регулярен израз до автомат
    операция за преобразуване на автомат до низ, съдържащ регулярен израз (теорема на Клини)

    детерминизация на автомат
    минимизация на автомат
    да се реализират операции, които проверяват дали езикът на даден автомат е:
        празен (не се разпознава нито една дума)
        краен (разпознават се краен брой думи)
        пълен (разпознават се всички думи)

*/