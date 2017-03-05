/*
 * MDP.h
 *
 *  Created on: 10 Feb 2017
 *      Author: elipol
 */

#ifndef SRC_MDP_H_
#define SRC_MDP_H_


#include "MC.h"

class MDP{
  public:

    typedef std::vector<transitiont> action;

    struct MDP_statet {
        MDP_statet(){input=0;}
        unsigned ID;
        bool init;
        unsigned input;
        std::vector<action> actions;
        std::vector<int>action_rewards;
    };

    //memoryless strategy: strategy[i] = j, pick action[j] in MDP_state[i].
  //  typedef unsigned actionNumber;
  //  std::vector<actionNumber> strategy;

    std::vector<MDP_statet> MDPstates;
    //note that slot 0 in modelparams is 1, and used
    //to represent known constants in param functions
    std::vector<fractiont> modelparams;
    std::vector<unsigned> parametercounts;
    std::vector<unsigned> inv_parametercounts;
    std::vector<fractiont> confidence;
    fractiont overall_confidence;

    std::vector<std::vector<std::pair <double, double > > >parameter_bounds;
    std::vector<std::string> parameter_results;

    unsigned verbose;
    unsigned trace_length;
    unsigned number_of_traces;
    unsigned num_int_samples; //for monte carlo integration

    //models
    int success=2; //success state in property
    MDP_statet get_init_state();
    void add_IDs();
    void initialise_all_counts();

    //parameter synthesis
    void outputPrism(std::ostream &);
    void callPrism();
    void prism_find(std::string &);
    bool result_bound_satisfied(unsigned , std::vector<double>& );
    bool is_in_range(std::vector<double>&);

    //strategy synthesis
    std::vector<unsigned> synthStrategy();
    void paramImportance();
    void assignActionRewards(std::vector<int> &);
    std::vector<unsigned> explicitStrategySynth();
    fractiont expectedInformationGain (std::vector<unsigned> &);

    //confidence calculation
    void getData(unsigned,std::vector<unsigned>&, random_distribution &, int);
    MC induceMarkovChain(std::vector<unsigned>&);
    void updateTransitionCounts(MC &, std::vector<unsigned>& );

    fractiont operator()();
};



MDP get_MDP();


#endif /* SRC_MDP_H_ */

