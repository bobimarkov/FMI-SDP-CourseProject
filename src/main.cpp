#include <iostream>
#include <fstream>

#include "Automaton.hpp"

/// Opens a text file and prints all rows which are recognized by the automaton.
bool readFile (std::string& path, Automaton automaton) {
    std::ifstream in (path, std::ios::in);

    if (!in.is_open()) {
        throw std::runtime_error("Invalid file path!");
    }

    std::string word;

    while (in.good()) {
        in >> word;
        if (automaton.recognize(word)) {
            std::cout << word << std::endl;
        }
    }

    in.close();
    return true;
}

int main (int argc, char** argv) {
    std::string filePath = argv[1];
    std::string regex = argv[2];

    Automaton automaton;
    automaton << regex;

    readFile(filePath, automaton);

    return 0;
}

