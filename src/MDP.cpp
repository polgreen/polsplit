
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

void MDP::num_states(int num_states)
{
  MDP_statet s;
  for(int i=0; i<num_states; i++)
  {
    MDPstates.push_back(s);
  }
}

void MDP::add_const_transition(int state, int action_num, int successor, double prob)
{
  fractiont f1 = double_to_fraction(prob);
  add_const_transition(state, action_num, successor, f1);
}

void MDP::add_const_transition(int state, int action_num,int successor, const fractiont& prob)
{
  if(MDPstates.size()<state)
    {
      std::cout<<"cannot add transitions between states that don't exist \n";
      throw std::exception();
    }
  action a1;
  for(int i=MDPstates[state].actions.size(); i<=action_num; i++)
    {MDPstates[state].actions.push_back(a1);}
  transitiont t1;
  t1.type=CONST;
  t1.prob=prob;
  t1.successor=successor;
  MDPstates[state].actions[action_num].push_back(t1);

}

void MDP::add_param_transition(int state, int action_num, int successor, const double param_multipliers[])
{
  std::vector<fractiont> fraction_mults;

  for(int i=0; i<modelparams.size(); i++)
  {
    fraction_mults.push_back(double_to_fraction(param_multipliers[i]));
  }
  add_param_transition(state, action_num, successor, fraction_mults);
}

void MDP::add_param_transition(int state, int action_num, int successor, const std::vector<fractiont>& param_multipliers)
{
  if(MDPstates.size()<state)
    {
      std::cout<<"cannot add transitions between states that don't exist \n";
      throw std::exception();
    }
  action a1;
  for(int i=MDPstates[state].actions.size(); i<=action_num; i++)
    {MDPstates[state].actions.push_back(a1);}
  transitiont t1;
  t1.type=FUNCTION;
  t1.successor=successor;
  std::pair<fractiont,unsigned> p1;
  for(int i=0; i<param_multipliers.size(); i++)
  {
    if(param_multipliers[i]!=0)
    {
     p1.first=param_multipliers[i];
     p1.second=i;
     t1.params.push_back(p1);
    }
  }

  MDPstates[state].actions[action_num].push_back(t1);
}

void MDP::add_remainder_transition(int state, int action_num, int successor)
{
  if(MDPstates.size()<state)
    {
      std::cout<<"cannot add transitions between states that don't exist \n";
      throw std::exception();
    }
  action a1;
  for(int i=MDPstates[state].actions.size(); i<=action_num; i++)
    {MDPstates[state].actions.push_back(a1);}

  transitiont t;
  t.type=REMAINDER;
  t.successor=successor;
  MDPstates[state].actions[action_num].push_back(t);
}

void MDP::make_state_init(int state)
{
  MDPstates[state].init=true;
}

void MDP::check()
{
  fractiont weight;
  fractiont state_sum;
  fractiont param_sum;
  if(MDPstates.size()==0)
    {std::cout<<"error in MC::check(), no states found \n";
    throw std::exception();}

  for(const auto &s: MDPstates)
  {
    if(s.ID>=MDPstates.size()){std::cout<<"StateIDs not properly assigned";
    throw std::exception();}

    for(const auto &a: s.actions)
    {
     state_sum.zero();
     if(a.size()==0){std::cout<<"empty action \n";
     throw std::exception();}

     for(const auto &t: a)
     {
       weight = weighting(t,a);
      if(weight.nom<0 || weight.denom<0)
        {std::cout<<"ERROR: transition S"<<s.ID<<"->S"<<t.successor<<" probability less than 0 \n";
        throw std::exception();}
      state_sum = weight + state_sum;
      if(state_sum.nom > state_sum.denom)
        {std::cout<<"ERROR: transitions from S"<<s.ID<<" sum to more than 1 \n";
        throw std::exception();}
      if(t.type==FUNCTION)
        { for(const auto &p: t.params)
          {
            param_sum = p.first + param_sum;
            if(param_sum.nom>param_sum.denom){std::cout<<"ERROR: parameter multipliers >1";
              throw std::exception();}
          }
        }
     }
    }
  }
}



MDP::MDP_statet MDP::get_init_state()
{
  bool found=false;
  MDP_statet result;
  for (const auto& s: MDPstates)
  {
    if(s.init==true && found==false)
    {
      found=true;
      result= s;
    }
    else if(s.init==true && found==true)
      {std::cout<<"error in get_init_state: found 2 initial states: "<<result.ID<<" and "<<s.ID<< "\n";
      throw std::exception();}
  }
  if(found==false)
  {
    std::cout<<"error in get_init_state: no initial state found \n";
       throw std::exception();
  }

return result;
}


fractiont MDP::weighting(const transitiont& t, const std::vector<transitiont>& action)
{
  fractiont sum, prod;
  sum.zero();


  switch(t.type)
  {
    case CONST: return t.prob; break;
    case FUNCTION:
      for(unsigned index=0; index<t.params.size(); index++)
            {
              prod = t.params[index].first*modelparams[t.params[index].second];
              sum = prod + sum;
            }
          return sum; break;
      case REMAINDER: return remainderWeight(action) ;break;
        default:
            throw std::exception();
    }

}



fractiont MDP::remainderWeight(const std::vector<transitiont>& action)
{
  fractiont sum_state;
  fractiont result;
  bool remainderfound =false;
  sum_state.zero();
  for(const auto & t: action)
   {
    if((t.type==REMAINDER) && remainderfound==false)
    {remainderfound=true;}
    else if((t.type==REMAINDER) && remainderfound==true)
    {std::cout<<"error, 2 transitions of type REMAINDER found \n";
     throw std::exception();}
    else{sum_state = sum_state + weighting(t, action);}
   }
  result.one();
  if(sum_state.nom>sum_state.denom || sum_state.nom<=0)
    {std::cout<<"error, invalid state found calculating remainder weight S \n";
    throw std::exception();}
  result = result-sum_state;
  return result;
}

statet MDP_to_MC_state(MDP::MDP_statet &mS, unsigned actionNumber)
{
  statet s;
  s.ID = mS.ID;
  s.init = mS.init;
  std::cout<<"actions size"<< mS.actions.size()<<", action "<<actionNumber<<std::endl;
  assert(actionNumber<mS.actions.size());
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
  std::cout<<"end of induce MC\n";
  return model;
}

void MDP::getData(unsigned tracelength,std::vector<unsigned>& strategy, random_distribution &rd, int integration_samples)
{
  MC model = induceMarkovChain(strategy);
  model.get_data(tracelength, rd); //get single trace
  model.confidencecalc(false,integration_samples); //update posterior distributions for single trace
  //copy these numbers back and forth is pretty inefficient, but this was a quick hack

  updateTransitionCounts(model, strategy);
  parametercounts = model.parametercounts;
  inv_parametercounts = model.inv_parametercounts;
  confidence = model.confidence;//transfer over posterior distribution
  overall_confidence = model.overall_confidence;
}

void MDP::updateTransitionCounts(MC & model, std::vector<unsigned>& strategy)
{
  for(unsigned i=0; i<MDPstates.size(); i++)
  {
    for(unsigned j=0; j<model.states[i].transitions.size(); j++)
    {
      MDPstates[i].actions[strategy[i]][j].count = model.states[i].transitions[j].count;
    }
  }
}

void MDP::initialise_all_counts()
{
  parametercounts.resize(modelparams.size());
  inv_parametercounts.resize(modelparams.size());
  confidence.resize(modelparams.size());
  overall_confidence.zero();
  for(int i=0; i<modelparams.size(); i++)
  {
    parametercounts[i]=1;
    inv_parametercounts[i]=1;
    confidence[i].zero();
  }
}


fractiont MDP::operator()()
{
  initialise_all_counts();

  callPrism();
  if(verbose>1)
    std::cout<<"collect data \n";
  random_distribution rd;
  rd.set_seed(0);
  int int_samples=100;
  for(unsigned n=0; n<number_of_traces; n++)
  {
    if(n==number_of_traces-1)
      int_samples = num_int_samples;
    std::vector<unsigned> strategy = synthStrategy();
    getData(trace_length, strategy, rd, int_samples);//and update posterior
  }

  if(verbose>0)
    std::cout<<"Final parameter counts: "<<parametercounts[1]<<" "<<inv_parametercounts[1]<<std::endl;

  //confidence was computed at end of each sampling, just return it
  return overall_confidence;
}

