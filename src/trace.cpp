#include <vector>
#include <random>
#include <iostream>
#include <cassert>
#include <functional>

#include "fraction.h"
#include "model.h"
#include "model_mdp.h"
#include "distributions.h"

double MC::Generate(const double from, const double to) {
    double result;
    result = (to - from) * rd.beta(1, 1) + from;
    return result;
}

tracet MC::gettrace() {
    tracet trace;
    statet state = get_init_state();
    bool gotnext = false;
    unsigned next = 0;
    trace.push_back(state);

    while (trace.size() < trace_length) {
        fractiont sum;
        sum.zero();
        //get total weighting of outgoing transitions
        for (const auto& t : state.transitions) { //std::cout<<"debug 3 \n"; 
            sum = sum + weighting(t, state);
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

        for (const auto& t : state.transitions) {
            mass = mass + weighting(t, state);
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
        trace.push_back(state);
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

tracet MDP::gettrace() {
    tracet trace;
    statet_a state = get_init_state();
    data_acq_strategy.clear();
    data_acq_strategy.resize(states.size());
    data_acq_strategy[getStateIndex(state.ID)] = rd.random_int(state.actions.size()); //randomized on the actions
    bool gotnext = false;
    unsigned next = 0;
    statet mc_state = state.getMCStateStruc(data_acq_strategy[getStateIndex(state.ID)]);
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
        data_acq_strategy[getStateIndex(state.ID)] = rd.random_int(state.actions.size());
        mc_state = state.getMCStateStruc(data_acq_strategy[getStateIndex(state.ID)]);
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

//make this pass by reference

//unsigned trace_count(unsigned s1, unsigned s2, tracet t) {
//    unsigned count;
//    for (unsigned i = 0; i < t.size(); i++) {
//        if (t[i].ID == s1 && t[i + 1].ID == s2) {
//            count++;
//        }
//        i++;
//    }
//    return count;
//}

void MC::get_trace_counts(tracet & trace) {
    for (unsigned i = 0; i < trace.size() - 1; i++) {
        if (states[getStateIndex(trace[i].ID)].transitions.size() == 0) {
            std::cout << "Error no transitions on state" << trace[i].ID << "\n";
            throw std::exception();
        }

        for (unsigned t = 0; t < states[getStateIndex(trace[i].ID)].transitions.size(); t++) {
            if (states[getStateIndex(trace[i].ID)].transitions[t].successor == trace[i + 1].ID) {
                states[getStateIndex(trace[i].ID)].transitions[t].count++;
                states[getStateIndex(trace[i].ID)].input++;
            }
        }
    }
    if (verbose > 1) {
        std::cout << "transition counts: \n";
        for (const auto s : states) {
            for (const auto t : s.transitions) {
                std::cout << "s" << s.ID << "-> s" << t.successor << ": " << t.count << "\n";
            }
        }
    }

}

void MDP::get_trace_counts(tracet & trace) {
    for (unsigned i = 0; i < trace.size() - 1; i++) {
        unsigned state_index = getStateIndex(trace[i].ID);        
        if (states[state_index].actions[data_acq_strategy[state_index]].first.size() == 0) {
            std::cout << "Error no transitions on state" << trace[i].ID << "\n";
            throw std::exception();
        }

        for (unsigned t = 0; t < states[state_index].actions[data_acq_strategy[state_index]].first.size(); t++) {
            if (states[state_index].actions[data_acq_strategy[state_index]].first[t].successor == trace[i + 1].ID) {                
                states[state_index].actions[data_acq_strategy[state_index]].first[t].count++;
                states[state_index].actions[data_acq_strategy[state_index]].second++;
            }
        }
    }
    if (verbose > 1) {
        std::cout << "transition counts: \n";
        for (const auto s : states) {
            for (unsigned i = 0; i < s.actions.size(); i++) {
                for (const auto t : s.actions[i].first) {
                    std::cout << "(s" << s.ID <<", a"<< i << " )-> s" << t.successor << ": " << t.count << " with action count: " << s.actions[i].second << " \n";
                }
            }
        }
    }
}

void MC::get_data() {
    tracet T = gettrace();
    get_trace_counts(T);
}

//void printtrace(tracet trace) {
//    std::cout << "\n";
//    for (const auto &s : trace) {
//        printstate(s);
//    }
//    std::cout << "\n";
//}

unsigned MC::getStateIndex(unsigned id) {
    unsigned index;
    for (unsigned i = 0; i < states.size(); i++) {
        if (states[i].ID == id) {
            index = i;
            break;
        }
    }

    return index;
}

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
