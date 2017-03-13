#include <assert.h>
#include "MDP.h"
#include "fraction.h"
#include "distributions.h"
#include "MC.h"

void MDP::add_IDs()
{
  for (unsigned i = 0; i < MDPstates.size(); i++)
  {
    MDPstates[i].ID = i;
  }
}

void MDP::num_states(const int num_states)
{
  MDP_statet s;
  for (int i = 0; i < num_states; i++)
  {
    MDPstates.push_back(s);
  }
}

void MDP::add_const_transition(const int state, const int action_num,
    const int successor, const double prob)
{
  fractiont f1 = double_to_fraction(prob);
  add_const_transition(state, action_num, successor, f1);
}

void MDP::add_const_transition(const int state, const int action_num,
    const int successor, const fractiont& prob)
{
  if (MDPstates.size() < state)
  {
    std::cout << "cannot add transitions between states that don't exist \n";
    throw std::exception();
  }
  action a1;
  for (int i = MDPstates[state].actions.size(); i <= action_num; i++)
  {
    MDPstates[state].actions.push_back(a1);
  }
  transitiont t1;
  t1.type = CONST;
  t1.prob = prob;
  t1.successor = successor;
  MDPstates[state].actions[action_num].push_back(t1);

}

void MDP::add_param_transition(const int state, const int action_num,
    const int successor, const double param_multipliers[])
{
  std::vector<fractiont> fraction_mults;

  for (int i = 0; i < modelparams.size(); i++)
  {
    fraction_mults.push_back(double_to_fraction(param_multipliers[i]));
  }
  add_param_transition(state, action_num, successor, fraction_mults);
}

void MDP::add_param_transition(const int state, const int action_num,
    const int successor, const std::vector<fractiont>& param_multipliers)
{
  if (MDPstates.size() < state)
  {
    std::cout << "cannot add transitions between states that don't exist \n";
    throw std::exception();
  }
  action a1;
  for (int i = MDPstates[state].actions.size(); i <= action_num; i++)
  {
    MDPstates[state].actions.push_back(a1);
  }
  transitiont t1;
  t1.type = FUNCTION;
  t1.successor = successor;
  std::pair<fractiont, unsigned> p1;
  for (int i = 0; i < param_multipliers.size(); i++)
  {
    if (param_multipliers[i] != 0)
    {
      p1.first = param_multipliers[i];
      p1.second = i;
      t1.params.push_back(p1);
    }
  }

  MDPstates[state].actions[action_num].push_back(t1);
}

void MDP::add_remainder_transition(const int state, const int action_num,
    const int successor)
{
  if (MDPstates.size() < state)
  {
    std::cout << "cannot add transitions between states that don't exist \n";
    throw std::exception();
  }
  action a1;
  for (int i = MDPstates[state].actions.size(); i <= action_num; i++)
  {
    MDPstates[state].actions.push_back(a1);
  }

  transitiont t;
  t.type = REMAINDER;
  t.successor = successor;
  MDPstates[state].actions[action_num].push_back(t);
}

void MDP::make_state_init(int state)
{
  MDPstates[state].init = true;
}

void MDP::check()
{
  fractiont weight;
  fractiont state_sum;
  fractiont param_sum;
  if (MDPstates.size() == 0)
  {
    std::cout << "error in MC::check(), no states found \n";
    throw std::exception();
  }

  for (const auto &s : MDPstates)
  {
    if (s.ID >= MDPstates.size())
    {
      std::cout << "StateIDs not properly assigned";
      throw std::exception();
    }

    for (const auto &a : s.actions)
    {
      state_sum.zero();
      param_sum.zero();
      if (a.size() == 0)
      {
        std::cout << "empty action \n";
        throw std::exception();
      }

      for (const auto &t : a)
      {
        weight = weighting(t, a);
        if (weight.nom < 0 || weight.denom < 0)
        {
          std::cout << "ERROR: transition S" << s.ID << "->S" << t.successor
              << " probability less than 0 \n";
          throw std::exception();
        }
        state_sum = weight + state_sum;
        if (state_sum.nom > state_sum.denom)
        {
          std::cout << "ERROR: transitions from S" << s.ID
              << " sum to more than 1 \n";
          throw std::exception();
        }
        if (t.type == FUNCTION)
        {
          for (const auto &p : t.params)
          {
            param_sum = p.first + param_sum;
            if (param_sum.nom > param_sum.denom)
            {
              std::cout << "ERROR: parameter multipliers > 1 for state" << s.ID
                  << "trans to" << t.successor << std::endl;
              std::cout << "param multiplier sum " << param_sum;
              throw std::exception();
            }
          }
        }
      }
    }
  }
}

MDP::MDP_statet MDP::get_init_state()
{
  bool found = false;
  MDP_statet result;
  for (const auto& s : MDPstates)
  {
    if (s.init == true && found == false)
    {
      found = true;
      result = s;
    }
    else if (s.init == true && found == true)
    {
      std::cout << "error in get_init_state: found 2 initial states: "
          << result.ID << " and " << s.ID << "\n";
      throw std::exception();
    }
  }
  if (found == false)
  {
    std::cout << "error in get_init_state: no initial state found \n";
    throw std::exception();
  }

  return result;
}

fractiont MDP::weighting(const transitiont& t,
    const std::vector<transitiont>& action)
{
  fractiont sum, prod;
  sum.zero();

  switch (t.type)
  {
    case CONST:
      return t.prob;
      break;
    case FUNCTION:
      for (unsigned index = 0; index < t.params.size(); index++)
      {
        prod = t.params[index].first * modelparams[t.params[index].second];
        sum = prod + sum;
      }
      return sum;
      break;
    case REMAINDER:
      return remainderWeight(action);
      break;
    default:
      throw std::exception();
  }

}

fractiont MDP::remainderWeight(const std::vector<transitiont>& action)
{
  fractiont sum_state;
  fractiont result;
  bool remainderfound = false;
  sum_state.zero();
  for (const auto & t : action)
  {
    if ((t.type == REMAINDER) && remainderfound == false)
    {
      remainderfound = true;
    }
    else if ((t.type == REMAINDER) && remainderfound == true)
    {
      std::cout << "error, 2 transitions of type REMAINDER found \n";
      throw std::exception();
    }
    else
    {
      sum_state = sum_state + weighting(t, action);
    }
  }
  result.one();
  if (sum_state.nom > sum_state.denom || sum_state.nom <= 0)
  {
    std::cout << "error, invalid state found calculating remainder weight "<<sum_state<<std::endl;
    throw std::exception();
  }
  result = result - sum_state;
  return result;
}

statet MDP_to_MC_state(MDP::MDP_statet &mS, const unsigned actionNumber)
{
  statet s;
  s.ID = mS.ID;
  s.init = mS.init;
  // std::cout<<"actions size"<< mS.actions.size()<<", action "<<actionNumber<<std::endl;
  assert(actionNumber < mS.actions.size());
  s.transitions = mS.actions[actionNumber];
  return s;
}

MC MDP::induceMarkovChain(const std::vector<unsigned>& strategy)
{
  MC model;
  model.modelparams = modelparams;
  assert(MDPstates.size() == strategy.size());
  if (verbose > 2)
    std::cout << "..induce Markov chain for strategy\n";

  for (unsigned i = 0; i < MDPstates.size(); i++)
  {
    statet s = MDP_to_MC_state(MDPstates[i], strategy[i]);
    model.states.push_back(s);
  }

  model.prior_a1=prior_a1;
  model.prior_a2=prior_a2;
 // std::cout<<"prior size "<<prior_a1.size()<<std::endl;
  model.verbose = verbose;
  model.trace_length = trace_length;
  model.number_of_traces = 1;
  //copy these numbers back and forth is pretty inefficient, but this was a quick hack
  model.parameter_bounds = parameter_bounds;
  model.parameter_results = parameter_results;
  model.parametercounts.resize(parametercounts.size());
  model.inv_parametercounts.resize(inv_parametercounts.size());
  model.param_confidence.resize(modelparams.size());
  return model;
}

void MDP::getData(const unsigned tracelength,
    const std::vector<unsigned>& strategy, random_distribution &rd,
    const int integration_samples)
{
  MC model = induceMarkovChain(strategy);
  model.get_data(tracelength, rd); //get single trace
  model.confidencecalc(true, integration_samples);

  //update prior with the new posterior:
  for(int i=1; i<modelparams.size(); i++)
  {
    prior_a1[i]+=model.parametercounts[i];
    prior_a2[i]+=model.inv_parametercounts[i];
    if(verbose>0)
      std::cout<<"updated prior for p"<<i<<" "<<prior_a1[i]<<" "<<prior_a2[i]<<std::endl;
  }
  overall_confidence = model.overall_confidence;
}


void MDP::initialise_all_counts()
{
  parametercounts.resize(modelparams.size());
  inv_parametercounts.resize(modelparams.size());
  overall_confidence.nom = 1;
  overall_confidence.denom = 2;

  for (int i = 0; i < modelparams.size(); i++)
  {
    parametercounts[i] = 0;
    inv_parametercounts[i] = 0;
  }

  if(prior_a1.size()==0)
  {
    prior_a1.resize(modelparams.size());
    prior_a2.resize(modelparams.size());
    for(auto &p: prior_a1)
      p=1;
    for(auto&p: prior_a2)
      p=1;
  }
  while(param_upper_bounds.size()<modelparams.size())
  {
  param_upper_bounds.push_back(1);
  param_lower_bounds.push_back(0);
  }
}

fractiont MDP::operator()(random_distribution &rd)
{
  initialise_all_counts();
  callPrism();
  if (verbose > 1)
    std::cout << "collect data \n";
  int int_samples = num_temp_int_samples;
  for (unsigned n = 0; n < number_of_traces; n++)
  {
    if (n == number_of_traces - 1)
      int_samples = num_int_samples;
    std::vector<unsigned> strategy = synthStrategy(rd);
    getData(trace_length, strategy, rd, int_samples);  //and update posterior
  }

  if (verbose > 0)
  {
    std::cout << "Final distribution counts: ";
    for (int i = 1; i < modelparams.size(); i++)
    {
      std::cout << "p" << i << ": " << prior_a1[i] << " "
          << prior_a2[i] << ", ";
    }
    std::cout << std::endl;
  }

  //confidence was computed at end of each sampling, just return it
  return overall_confidence;
}

