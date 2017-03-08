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

fractiont MDP::expectedInformationGain(std::vector<unsigned> & strategy,
    random_distribution &rd)
{
  if (verbose > 2)
    std::cout << "Computing expected information gain\n";
  MC model = induceMarkovChain(strategy);
  if (verbose > 2)
  {
    std::cout << "Induced Markov Chain: \n";
    model.outputMC(std::cout);
  }

 std::vector<statet> current_states;
  current_states.push_back(model.get_init_state());
  fractiont expected_confidence;

  std::vector<fractiont> state_inputs(model.states.size());
  std::vector<fractiont> next_state_inputs(model.states.size());
  for (auto s : state_inputs)
  {
    s.zero();
  }
  for (auto s : next_state_inputs)
  {
    s.zero();
  }

  state_inputs[model.get_init_state().ID].one();
  std::vector<std::vector<fractiont> > transitioncounts;
  std::vector<fractiont> expected_param_counts(model.modelparams.size());
  std::vector<fractiont> expected_invparam_counts(model.modelparams.size());
  transitioncounts.resize(model.states.size());

  for (int s = 0; s < model.states.size(); s++)
  {
    transitioncounts[s].resize(model.states[s].transitions.size());
    for (auto &t : transitioncounts[s])
    {
      t.zero();
    }
  }

  for (int i = 1; i < modelparams.size(); i++)
  {
    expected_param_counts[i].nom = (int) parametercounts[i];
    expected_param_counts[i].denom = 1;
    expected_invparam_counts[i].nom = (int) parametercounts[i];
    expected_invparam_counts[i].denom = 1;
    model.modelparams[i].nom = (int) parametercounts[i]+prior_a2[i];
    model.modelparams[i].denom = (int) parametercounts[i]
        + (int) inv_parametercounts[i]+prior_a2[i];
    if(fraction_to_double(model.modelparams[i])>param_upper_bounds[i])
    {
      model.modelparams[i].nom=model.modelparams[i].denom*param_upper_bounds[i];
    }
    else if(fraction_to_double(model.modelparams[i])<param_lower_bounds[i])
    {
      model.modelparams[i].nom=model.modelparams[i].denom*param_lower_bounds[i];
    }
  }

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
      double p1 = (double) expected_param_counts[i].nom
          / (double) expected_param_counts[i].denom;
      double p2 = (double) expected_invparam_counts[i].nom
          / (double) expected_invparam_counts[i].denom;
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
    std::cout << "change of "
        << frac_abs(overall_confidence - expected_confidence) << std::endl;
  }

  return frac_abs(overall_confidence - expected_confidence);
  fractiont result;
  result.one();
  return result;
}

std::vector<unsigned> MDP::explicitStrategySynth(random_distribution& rd)
{
  //explicitly compute expected counts for every strategy
  std::vector<std::vector<unsigned> > strategies;
  std::vector<int> expected_counts;
  strategies.resize(1);
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

  int max_strategy = 0;
  fractiont info_gain;
  fractiont max_info_gain = 0;
  for (int s = 0; s < strategies.size(); s++)
  {
    info_gain = expectedInformationGain(strategies[s], rd);
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
    strategy[i] = (unsigned) (MDPstates[i].actions.size() * rd.beta(1, 1));
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

