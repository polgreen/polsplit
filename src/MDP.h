/*
 * MDP.h
 *
 *  Created on: 10 Feb 2017
 *      Author: elipol
 */

#ifndef SRC_MDP_H_
#define SRC_MDP_H_


#include "model.h"




class MDP{
  public:
    typedef std::vector<transitiont> action;

    struct MDP_statet {
        unsigned ID;
        bool init;
        unsigned label;
        std::vector<action> actions;
    };

    //memoryless strategy: strategy[i] = j, pick action[j] in MDP_state[i].
  //  typedef unsigned actionNumber;
  //  std::vector<actionNumber> strategy;

    std::vector<MDP_statet> MDPstates;
    std::vector<fractiont> modelparams;
    std::vector<unsigned> parametercounts;
    std::vector<unsigned> inv_parametercounts;
    std::vector<fractiont> confidence;
    fractiont overall_confidence;

    MC induceMarkovChain(std::vector<unsigned>&);
};






#endif /* SRC_MDP_H_ */
