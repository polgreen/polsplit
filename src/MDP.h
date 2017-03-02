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
        MDP_statet(){input=0;inputknown=true; outputknown=true;}
        unsigned ID;
        bool init;
        unsigned label;
        bool outputknown;
        bool inputknown;
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
    unsigned number_samples=1000; //for monte carlo integration

    std::vector<unsigned> synthStrategy();
    void getData(unsigned,std::vector<unsigned>&, random_distribution &);
    void outputPrism(std::ostream &);
    void callPrism();
    void prism_find(std::string &);
    bool is_in_range(std::vector<double> &);
    bool result_bound_satisfied(unsigned , std::vector<double>& );

    void get_data(unsigned);
    MC induceMarkovChain(std::vector<unsigned>&);
    fractiont operator()();
    void paramImportance();
    void assignActionRewards(std::vector<int> &);
    std::vector<unsigned> brute_force_strategySynthesis();
    MDP_statet get_init_state();
    void add_IDs();


    int success=2;

};



MDP get_MDP();


#endif /* SRC_MDP_H_ */

