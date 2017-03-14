/*****************************\
          (\.---./)    
 /.-.-.\
          /| 0_0 |\
         |_`-(v)-'_|
 \`-._._.-'/      .-.
 -~-(((.`-\_|_/-'.)))-~' <_
 `.     .'
 `._.'
 BaeVer
 Bayesian Verification for DTMCs     
 -----~--~---~~~----~-`.-;~
 elizabeth.polgreen@cs.ox.ac.uk
 \********************************/

#include "MC.h"
#include <vector>
#include <random>
#include <iostream>
#include <cassert>
#include "fraction.h"

void MC::add_IDs()
{
  for (unsigned i = 0; i < states.size(); i++)
  {
    states[i].ID = i;
  }
}


void MC::check()
{
  fractiont weight;
  fractiont state_sum;
  fractiont param_sum;
  if (states.size() == 0)
  {
    std::cout << "error in MC::check(), no states found \n";
    throw std::exception();
  }

  for (const auto &s : states)
  {
    if (s.ID >= states.size())
    {
      std::cout << "StateIDs not properly assigned";
      throw std::exception();
    }
    state_sum.zero();
    for (const auto &t : s.transitions)
    {
      weight = weighting(t, s);
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
            std::cout << "ERROR: parameter multipliers >1";
            throw std::exception();
          }
        }
      }
    }
  }
}

bool MC::check_for_parameters()
{
  for(const auto &s: states)
  {
    for (const auto &t :s.transitions)
    {
      if(t.type==FUNCTION)
        return true;
    }
  }
return false;
}

std::vector<std::pair<statet, unsigned> > MC::get_parameterised_states()
{

  if (states.size() == 0)
  {
    std::cout << "no model states found\n";
    throw std::exception();
  }
  std::vector<std::pair<statet, unsigned> > result;

  //vector of < vectors of <state and transition index >
  //for(const auto & v: result) //initialise result vector of vectors to empty for each parameter
  //{v = {};}
  std::pair<statet, unsigned> pair;
  bool found = false;

  for (auto &s : states)
  {
    for (unsigned t = 0; t < s.transitions.size(); t++)
    {
      if (s.transitions[t].type == FUNCTION)
      {
        if (verbose > 1)
          std::cout << "Found param state " << s.ID << " , param transition to "
              << s.transitions[t].successor << "\n";
        pair.first = s;
        pair.second = t;
        result.push_back(pair);
        found = true;
      }
    }
  }

  if (found == false)
  {
    if(verbose>2)
      std::cout<< " no parameterised states found \n";
    throw std::exception();
  }

  return result;
}

fractiont MC::weighting(const transitiont& t, const statet& s)
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
      return remainderWeight(s);
      break;
    default:
      std::cout << "error, state" << s.ID << " transition to " << t.successor
          << " type" << t.type << " unknown \n";
      throw std::exception();
  }

}

fractiont MC::remainderWeight(const statet& s)
{
  fractiont sum_state;
  fractiont result;
  bool remainderfound = false;
  sum_state.zero();
  for (const auto & t : s.transitions)
  {
    if ((t.type == REMAINDER) && remainderfound == false)
    {
      remainderfound = true;
    }
    else if ((t.type == REMAINDER) && remainderfound == true)
    {
      std::cout << "error, 2 transitions of type REMAINDER found on S" << s.ID
          << "\n";
      throw std::exception();
    }
    else
    {
      sum_state = sum_state + weighting(t, s);
    }
  }
  result.one();
  if (sum_state.nom > sum_state.denom || sum_state.nom <= 0)
  {
    std::cout<<"state sum "<<sum_state<<std::endl;
    std::cout << "error, invalid state found calculating remainder weight S"
        << s.ID << " \n";
    throw std::exception();
  }
  result = result - sum_state;
  return result;
}

statet MC::get_init_state()
{
  bool found = false;
  statet result;
  for (const auto& s : states)
  {
    if (s.init == true && found == false)
    {
      found = true;
      result = s;
    }
    else if (s.init == true && found == true)
    {
      std::cout << "error in get_init_state: found 2 initial states \n";
      throw std::exception();
    }
  }
  //if we get here throw exception
  if (found == false)
  {
    std::cout << "error in get_init_state: no initial state found \n";
    throw std::exception();
  }
  return result;
}

void printstate(statet& s)
{
  std::cout << s.ID << ", ";
}

void MC::outputMC(std::ostream &out)
{
  fractiont result;
  out << "\nMARKOV CHAIN: \n";
  out << "parameters: ";
  out<<"Number of states "<<states.size()<<"\n";
  for (auto &p : modelparams)
  {
    p.output(out);
    out << " ";
  }
  out << "\nSTATES \n";
  for (const auto &s : states)
  {
    out << "\nS" << s.ID;

    //if(s.inputknown==true){out<<" input known ";}
    //if(s.outputknown==true){out<<" output known ";}
    //(s.inputknown==false){out<<" input unknown ";}
    //if(s.outputknown==false){out<<" output unknown ";}
    out << ": \n";
    out << s.input << " input count \n";
    for (const auto & t : s.transitions)
    {
      if (t.type == FUNCTION)
      {
        out << "FUNCTION ";
      }
      out << "transition to S";
      out << t.successor << " weighting: ";
      result = weighting(t, s);
      if (result.nom == 0)
      {
        std::cout << "ERROR ZERO VALUE RETURNED";
      }
      result.output(out);
      out << ", count: " << t.count;
      out << "\n";

    }
  }
  out << "\n\n";
}

void MC::initialise_all_counts()
{
  parametercounts.resize(modelparams.size());
  inv_parametercounts.resize(modelparams.size());
  param_confidence.resize(modelparams.size());
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

fractiont MC::operator()(random_distribution &rd)
{
  MC model=*this;
  return model(rd, true);
}

fractiont MC::operator()(random_distribution &rd, bool do_synthesis)
{
  if(do_synthesis)
  {
    initialise_all_counts();
    std::cout << "Calling PRISM for parameter synthesis ... \n";
    callPrism();
    std::cout << "\n";
  }

  std::cout << "Generating data from underlying system ... \n";
  //get data from model

  for (unsigned n = 0; n < number_of_traces; n++)
  {
    get_data(trace_length, rd);
  }
  //do confidence calculation
  std::cout << "Computing confidence system satisfies property ... \n";
  return confidencecalc(true, num_int_samples, rd);
}

