/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <vector>
#include <random>
#include <iostream>
#include <cassert>
#include <functional>

#include "fraction.h"
#include "model.h"
#include "model_mdp.h"
#include "distributions.h"

unsigned MDP::getStateIndex(unsigned id) {
    unsigned index;
    for (unsigned i = 0; i < states.size(); i++) {
        if (states[i].ID == id) {
            index = i;
            break;
        }
    }

    return index;
}

void MDP::get_trace_counts(tracet & trace) {
    for (unsigned i = 0; i < trace.size() - 1; i++) {
        unsigned state_index = getStateIndex(trace[i].ID);
        if (states[state_index].actions[data_acq_strategy[i]].first.size() == 0) {
            std::cout << "Error no transitions on state" << trace[i].ID << "\n";
            throw std::exception();
        }

        for (unsigned t = 0; t < states[state_index].actions[data_acq_strategy[i]].first.size(); t++) {
            if (states[state_index].actions[data_acq_strategy[i]].first[t].successor == trace[i + 1].ID) {
                states[state_index].actions[data_acq_strategy[i]].first[t].count++;
                states[state_index].actions[data_acq_strategy[i]].second++;
            }
        }
    }
    if (verbose > 1) {
        std::cout << "transition counts: \n";
        for (const auto s : states) {
            for (unsigned i = 0; i < s.actions.size(); i++) {
                for (const auto t : s.actions[i].first) {
                    std::cout << "(s" << s.ID << ", a" << i << " )-> s" << t.successor << ": " << t.count << " with action count: " << s.actions[i].second << " \n";
                }
            }
        }
    }
}

tracet MDP::gettrace() {
    tracet trace;
    statet_a state = get_init_state();
    data_acq_strategy.clear();
    data_acq_strategy.resize(trace_length);
    data_acq_strategy[trace.size()] = rd.random_int(state.actions.size()); //randomized on the actions
    bool gotnext = false;
    unsigned next = 0;
    statet mc_state = state.getMCStateStruc(data_acq_strategy[trace.size()]);
    trace.push_back(mc_state); //push the state with the chosen action
    while (trace.size() < trace_length) {
        fractiont sum;
        sum.zero();
        //get total weighting of outgoing transitions
        for (const auto& t : mc_state.transitions) { //std::cout<<"debug 3 \n"; 
            sum = sum + weighting(t, mc_state);
        }
        if (verbose > 1) {
            std::cout << "Sum weight for transitions of state s" << state.ID;
            std::cout << " is: " << sum << "\n";
        }
        //std::uniform_int_distribution<unsigned> distribution(0,100);
        fractiont random;
        random.nom = Generate(0, 100);
        random.denom = 100;
        fractiont mass;
        fractiont subtraction;
        mass.zero();

        for (const auto& t : mc_state.transitions) {
            mass = mass + weighting(t, mc_state);
            subtraction = mass - random;
            if ((subtraction) > 0) {
                next = t.successor;
                gotnext = true;
                break;
            }
        }

        if (gotnext == false) {
            std::cout << "ERROR STATE NOT FOUND";
            throw std::exception();
        }
        if (state.ID >= states.size() || next >= states.size()) {
            std::cout << "error in get trace";
            throw std::exception();
        }

        state = states[getStateIndex(next)];
        data_acq_strategy[trace.size()] = rd.random_int(state.actions.size());
        mc_state = state.getMCStateStruc(data_acq_strategy[trace.size()]);
        trace.push_back(mc_state);

    }
    if (verbose > 1) {
        std::cout << "trace: ";
        for (const auto s : trace) {
            std::cout << " s" << s.ID;
        }
        std::cout << "\n";
    }

    return trace;

}