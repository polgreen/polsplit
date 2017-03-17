/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   model_mdp.h
 * Author: brian
 *
 * Created on March 8, 2017, 9:05 PM
 */

#ifndef MODEL_MDP_H
#define MODEL_MDP_H
#include "model.h"
#include <tuple>

class MDP : public MC {
public:
    typedef std::pair<std::vector<transitiont>, unsigned> action;

    struct statet_a : statet {
        std::vector<action> actions;
        statet getMCStateStruc(int);
        unsigned sum_outputs(unsigned);
    };

    std::vector<statet_a> states;
    std::vector<unsigned> data_acq_strategy; //Data acquisition strategy
    unsigned strategy_type; //Interface Control Methods  
    unsigned finiteMemMode; //Switch to finite memory mode: 0-> off, 1-> randomized finite memory
    void outputPRISM(std::ostream &out);
    void synthStrategy();
    void randomStrategy();
    void randomFntMemStrategy();
    statet_a get_init_state();
    tracet gettrace();
    void get_trace_counts(tracet&);
    unsigned getStateIndex(unsigned);
    void confidencecalc();
    std::vector< std::tuple <statet_a, unsigned, unsigned> > get_parameterised_states();
    void sample_D_star(std::vector< std::tuple <statet_a, unsigned, unsigned> > &);

};

class MDP_cmdvars {
public:
    unsigned strategy_type_cap;
    void add_MDP_cmd_options(int argc, const char *argv[], int, int*);
    void display_MDP_cmd_options();
    void init_process(int, int, int, long);
};
void add_MDP_help();


MC induceMarkovChain(MDP);
MDP get_MDP_instance();
MDP_cmdvars get_MDP_cmdvars_instance();
#endif /* MODEL_MDP_H */

