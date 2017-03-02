
#include <assert.h>
#include "MDP.h"
#include "fraction.h"
#include "distributions.h"
#include "MC.h"


void MDP::add_IDs ()
{
  for(unsigned i=0; i<MDPstates.size(); i++)
  {
    MDPstates[i].ID = i;
  }
}


MDP::MDP_statet MDP::get_init_state()
{
  bool found;
  for (const auto& s: MDPstates)
  {
    if(s.init==true && found==false)
    {return s;}
    else if(s.init==true && found==true)
      {std::cout<<"error in get_init_state: found 2 initial states \n";
      throw std::exception();}
  }
 //if we get here throw exception
  std::cout<<"error in get_init_state: no initial state found \n";
     throw std::exception();

}


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
  //copy these numbers back and forth is pretty inefficient, but this was a quick hack
  model.parameter_bounds = parameter_bounds;
  model.parameter_results = parameter_results;
  model.parametercounts = parametercounts;
  model.inv_parametercounts = inv_parametercounts;
  model.confidence = confidence;
  return model;
}

void MDP::getData(unsigned tracelength,std::vector<unsigned>& strategy)
{
  MC model = induceMarkovChain(strategy);
  model.get_data(tracelength); //get single trace
  model.confidencecalc(false,number_samples); //update posterior distributions for single trace
  //copy these numbers back and forth is pretty inefficient, but this was a quick hack

  parametercounts = model.parametercounts;
  inv_parametercounts = model.inv_parametercounts;
  confidence = model.confidence;//transfer over posterior distribution
}



fractiont MDP::operator()()
{
  confidence.resize(modelparams.size());
  parametercounts.resize(modelparams.size());
  inv_parametercounts.resize(modelparams.size());

  callPrism();
  if(verbose>1)
    std::cout<<"collect data \n";
  for(unsigned n=0; n<number_of_traces; n++)
  {
    std::vector<unsigned> strategy = synthStrategy();
    getData(trace_length, strategy); //and update posterior
  }

  //confidence was computed at end of each sampling, just return it
  return overall_confidence;
}

