/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "model.h"
#include "model_mdp.h"

void MDP::synthStrategy() {
    data_acq_strategy.clear();
    data_acq_strategy.resize(states.size());


    switch (strategy_type) {

        case 1:
            if (verbose > 1)
                std::cout << "Always pick the first action at a state as the strategy" << std::endl;
            for (auto &s : data_acq_strategy) {
                s = 0;
            }
            break;
        default:
            std::cout << "ERROR no strategy method selected\n";
            throw std::exception();
    }
}

MC induceMarkovChain(MDP m) {
    if (m.verbose > 2)
        std::cout << "..induce Markov chain for strategy\n";

    MC model = m;

    for (unsigned i = 0; i < m.states.size(); i++) {
        statet s;
        s.ID = m.states[i].ID;
        s.init = m.states[i].init;
        s.transitions = m.states[i].actions[m.data_acq_strategy[i]];
        model.states.push_back(s);
    }
    return model;
}