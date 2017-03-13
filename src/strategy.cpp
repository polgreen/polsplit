/*
 * strategy.cpp
 *
 *  Created on: 12 Feb 2017
 *      Author: elipol
 */

#include "MDP.h"
#include <vector>
#include <cstdlib> //abs
#include <algorithm> //std::sort
#include <assert.h> //assert

std::vector<fractiont> MDP::expected_parameters()
{
  std::vector<fractiont> expected_params(modelparams.size());
  for(int i=0; i<modelparams.size(); i++)
  {
    expected_params[i].nom = (int) parametercounts[i]+prior_a1[i];
    expected_params[i].denom = (int) inv_parametercounts[i]+(int)parametercounts[i]
                                        +(int)prior_a1[i]+(int)prior_a2[i];
      if(fraction_to_double(expected_params[i])>param_upper_bounds[i])
      {
        expected_params[i].nom=(double)expected_params[i].denom*param_upper_bounds[i];
      }
      else if(fraction_to_double(expected_params[i])<param_lower_bounds[i])
      {
        expected_params[i].nom=(double)expected_params[i].denom*param_lower_bounds[i];
      }
  }
  return expected_params;
}

std::vector<std::vector<fractiont> > MC::transitioncountvector()
{
  std::vector<std::vector<fractiont>> transitioncounts;
  transitioncounts.resize(states.size());
  for (int s = 0; s < states.size(); s++)
  {
    transitioncounts[s].resize(states[s].transitions.size());
  }
  return transitioncounts;
}


fractiont MDP::expectedInformationGain(const int strategy,
    random_distribution &rd)
{
 fractiont result;
  result.zero();
  if(strategy_gain[strategy]==false)
    return result;

  if (verbose > 2)
    std::cout << "Computing expected information gain\n";
  MC model = induceMarkovChain(strategies[strategy]);
  model.modelparams=expected_parameters();
  if (verbose > 2)
  {
    std::cout << "Induced Markov Chain: \n";
    model.outputMC(std::cout);
  }

  std::vector<statet> current_states;
  current_states.push_back(model.get_init_state());
  fractiont expected_confidence=overall_confidence;
  std::vector<fractiont> state_inputs(model.states.size());
  std::vector<fractiont> next_state_inputs(model.states.size());
  state_inputs[model.get_init_state().ID].one();
  std::vector<std::vector<fractiont> > transitioncounts = model.transitioncountvector();
  std::vector<fractiont> expected_param_counts(parametercounts.size());//=intvector_to_fractions(parametercounts);
  std::vector<fractiont> expected_invparam_counts(inv_parametercounts.size());//=intvector_to_fractions(inv_parametercounts);

  for (int i = 0; i < trace_length; i++)
  {
    for (int s = 0; s < state_inputs.size(); s++)
    {
      if (state_inputs[s] != 0)
      {
        for (int t = 0; t < model.states[s].transitions.size(); t++)
        {
          fractiont expected_count = model.weighting(
              model.states[s].transitions[t], model.states[s]);

          transitioncounts[s][t] = transitioncounts[s][t] + expected_count;
          next_state_inputs[model.states[s].transitions[t].successor] =
              state_inputs[model.states[s].transitions[t].successor]
                  + expected_count;
          if (model.states[s].transitions[t].type == FUNCTION)
          {
            for (int p = 0; p < model.states[s].transitions[t].params.size();
                p++)
            {
              int param_num = model.states[s].transitions[t].params[p].second;
              if (param_num == 0)
                break;
              expected_param_counts[param_num] =
                  expected_param_counts[param_num] + expected_count;
              expected_invparam_counts[param_num] =
                  expected_invparam_counts[param_num]
                      + state_inputs[s]
                          * model.states[s].transitions[t].params[p].first
                          * (1 - expected_count);
            }
          }
        }
      }
    }
    for (int j = 0; j < next_state_inputs.size(); j++)
      state_inputs[j] = next_state_inputs[j];
  }

//compute parameter confidence
  if (verbose > 1)
    for (int i = 1; i < modelparams.size(); i++)
      std::cout << "Computed expected parameter " << i << " counts: "
          << expected_param_counts[i] << " " << expected_invparam_counts[i]
          << std::endl;
  for (int i = 0; i < 10; i++)
  {
    std::vector<double> sample;
    for (unsigned i = 1; i < modelparams.size(); i++)
    {
      double p1 = fraction_to_double(expected_param_counts[i]);
      double p2 = fraction_to_double(expected_invparam_counts[i]);
      if(verbose>1)
        std::cout<<"distribution "<<p1<<" "<<p2<<std::endl;
      sample.push_back(rd.beta(p1 +  prior_a1[i], p2 + prior_a2[i]));
    }
    if (model.is_in_range(sample, false))
    {
      expected_confidence.nom++;
    }
    expected_confidence.denom++;
  }
  if (verbose > 1)
  {
    std::cout << "expected Strategy confidence  = " << expected_confidence
        << "\n";
  }
  return frac_abs(overall_confidence - expected_confidence);
}

std::vector<unsigned> MDP::explicitStrategySynth(random_distribution& rd)
{
  //explicitly compute expected counts for every strategy
  if(strategies.size()==0)
  {
    strategies.resize(1);
    strategy_gain.push_back(true);
    //get all strategies
    for (int i = 0; i < MDPstates.size(); i++)
    {
      for (auto &s : strategies)
      {
        s.push_back(0);
      }
      if (MDPstates[i].actions.size() > 1)
      {
        int currentsize = strategies.size();
        for (int j = 1; j < MDPstates[i].actions.size(); j++)
        {
          for (int k = 0; k < currentsize; k++)
          {
            strategy_gain.push_back(true);
            strategies.push_back(strategies[k]);
            strategies[currentsize + k * j - 1][i] = j;
          }
        }
      }
    }

    if (verbose > 1)
    {
      std::cout << "All possible strategies: \n";
      for (const auto &s : strategies)
      {
        std::cout << "strategy ";
        for (unsigned i = 0; i < s.size(); i++)
          std::cout << "s" << i << "-a" << s[i] << " ";
        std::cout << "\n";
      }
    }
  }

  int max_strategy = 0;
  fractiont info_gain;
  fractiont max_info_gain = 0;
  for (int s = 0; s < strategies.size(); s++)
  {
    info_gain = expectedInformationGain(s, rd);
    if (info_gain > max_info_gain)
      max_strategy = s;
    max_info_gain = info_gain;
  }
  //get expected data counts for all
  return strategies[max_strategy];
}

std::vector<unsigned> MDP::randomStrategySynth(random_distribution &rd)
{
  std::vector<unsigned> strategy;
  strategy.resize(MDPstates.size());
  for (int i = 0; i < MDPstates.size(); i++)
  {
    strategy[i] = rd.random_int(MDPstates[i].actions.size());// (unsigned) (MDPstates[i].actions.size() * rd.beta(1, 1));
  }

  return strategy;
}

std::vector<unsigned> MDP::synthStrategy()
{
  std::vector<unsigned> strategy;
  // paramImportance();
  //do strategy synthesis (via prismgames?)
  strategy.resize(MDPstates.size());
  random_distribution rd;

  switch (strategy_type)
  {
    case 0:
      if (verbose > 1)
        std::cout << "Explicit strategy synth" << std::endl;
      explicitStrategySynth(rd);
      break;
    case 1:
      if (verbose > 1)
        std::cout << "Pick the first action strategy synth" << std::endl;
      for (auto s : strategy)
      {
        s = 0;
      }
      break;
    case 2:
      randomStrategySynth(rd);
      if (verbose > 1)
        std::cout << "Randomized strategy synth" << std::endl;
      break;
    default:
      std::cout << "ERROR no strategy method selected\n";
      throw std::exception();
  }

  if (verbose > 1)
  {
    std::cout << "Memoryless strategy: ";
    for (const auto s : strategy)
      std::cout << " a" << s;
    std::cout << "\n";
  }
  return strategy;
}

