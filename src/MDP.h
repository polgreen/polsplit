/*
 * MDP.h
 *
 *  Created on: 10 Feb 2017
 *      Author: elipol
 */

#ifndef SRC_MDP_H_
#define SRC_MDP_H_

#include "MC.h"

class MDP
{
  public:

    typedef std::vector<transitiont> action;

    struct MDP_statet
    {
        MDP_statet()
        {
          input = 0;
          init = false;
        }
        unsigned ID;
        bool init;
        unsigned input;
        std::vector<action> actions;
        std::vector<int> action_rewards;
    };

    //memoryless strategy: strategy[i] = j, pick action[j] in MDP_state[i].
    std::vector<MDP_statet> MDPstates;
    //note that slot 0 in modelparams is 1, and used
    //to represent known constants in param functions
    std::vector<double> param_lower_bounds;
    std::vector<double> param_upper_bounds;
    std::vector<fractiont> modelparams;
    std::vector<unsigned> parametercounts;
    std::vector<unsigned> inv_parametercounts;
    fractiont overall_confidence;

    std::vector<std::vector<std::pair<double, double> > > parameter_bounds;
    std::vector<std::string> parameter_results;

    int strategy_type; //explicit synth=0, action 0 always=1, random=2;
    unsigned verbose;
    unsigned trace_length;
    unsigned number_of_traces;
    unsigned num_int_samples; //for monte carlo integration
    unsigned num_temp_int_samples = 100; //for integration for strategy synth
    bool need_state_splitting = true;

    //models
    int success = 2; //success state in property
    MDP_statet get_init_state();
    void add_IDs();
    void initialise_all_counts();
    void num_states(const int);
    void add_const_transition(const int, const int, const int,
        const fractiont&);
    void add_const_transition(const int, const int, const int, double);
    void add_param_transition(const int, const int, const int, const double[]);
    void add_param_transition(const int, const int, const int,
        const std::vector<fractiont>&);
    void add_remainder_transition(const int, const int, const int);
    void make_state_init(int);
    fractiont weighting(const transitiont&, const std::vector<transitiont>&);
    fractiont remainderWeight(const std::vector<transitiont>&);
    void check();

    //parameter synthesis
    void outputPrism(std::ostream &);
    void callPrism();
    void prism_find(const std::string &);

    //strategy synthesis
    std::vector<unsigned> synthStrategy();
    std::vector<unsigned> randomStrategySynth(random_distribution &);
    std::vector<unsigned> explicitStrategySynth(random_distribution &);
    fractiont expectedInformationGain(std::vector<unsigned> &,
        random_distribution &);

    //confidence calculation
    void getData(const unsigned, const std::vector<unsigned>&,
        random_distribution &, const int);
    MC induceMarkovChain(const std::vector<unsigned>&);
    void updateTransitionCounts(MC &, const std::vector<unsigned>&);

    fractiont operator()();
};

MDP get_MDP(int);

#endif /* SRC_MDP_H_ */

