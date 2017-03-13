#include <vector>
#include <random>
#include <iostream>
#include <cassert>
#include <functional>

#include "fraction.h"
#include "model.h"
#include "distributions.h"

double Generate(const double from, const double to, random_distribution& dist) {
    double result;
    result = (to - from) * dist.beta(1, 1) + from;
    return result;
}

tracet MC::gettrace(random_distribution& rd) {
    tracet trace;
    std::vector<std::vector<unsigned> > count;
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
        random.nom = Generate(0, 100, rd);
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

void MC::get_data(random_distribution & rd) {
    tracet T = gettrace(rd);
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
