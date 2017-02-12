/*
 * MDP.cpp

 *
 *  Created on: 12 Feb 2017
 *      Author: elipol
 */
#include <assert.h>
#include "MDP.h"
#include "fraction.h"
#include "distributions.h"
#include "model.h"



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

  return model;
}
