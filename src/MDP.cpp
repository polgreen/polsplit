/*
 * MDP.cpp

 *
 *  Created on: 12 Feb 2017
 *      Author: elipol
 */
#ifdef MDP
#include <assert.h>
#include "MDP.h"
#include "fraction.h"
#include "distributions.h"
#include "MC.h"



statet MDP_to_MC_state(MDP::MDP_statet &mS, unsigned actionNumber)
{
  statet s;
  s.ID = mS.ID;
  s.init = mS.init;
  s.label = mS.label;
  s.transitions  = mS.actions[actionNumber];
  return s;
}


MC MDP::induceMarkovChain(std::vector<unsigned>& strategy)
{
  MC model;
  model.modelparams = modelparams;
  assert(MDPstates.size()==strategy.size());

  for(unsigned i=0; i<MDPstates.size(); i++)
  {
    statet s =  MDP_to_MC_state(MDPstates[i], strategy[i]);
    model.states.push_back(s);
  }

  model.verbose = verbose;
  model.trace_length = trace_length;
  model.number_of_traces = 1;
  return model;
}



fractiont MDP::operator()()
{
  fractiont confidence;
  callPrism();
  for(unsigned n=0; n<number_of_traces; n++)
  {
    //synth strategy
    //induce MC
    //get data
    //update posterior
  }
  //compute confidence
  return confidence;
}
#endif //ifdef MDP
