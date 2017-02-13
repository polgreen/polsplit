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

struct sort_pred {
    bool operator()(const std::pair<int,int> &left, const std::pair<int,int> &right) {
        return left.second > right.second;
    }
};

void MDP::assignActionRewards(std::vector<int> &param_rewards)
{
  assert(param_rewards.size()==modelparams.size());
  for (auto &s: MDPstates)
  {
    for(int i=0; i<s.actions.size(); i++)
    {
      s.action_rewards[i]=0;
      for(const auto &t: s.actions[i])
      {
        if(t.type==FUNCTION)
        {
          for(const auto &p: t.params)
            {s.action_rewards[i]+=param_rewards[p.second];}
        }
      }
    }
  }
}

void MDP::paramImportance()
{
  //difference between (confidence) and (1-confidence) for each parameter
  std::vector<std::pair<int, int>> importance;
  importance.resize(confidence.size());

  for(int i=1; i<confidence.size(); i++)
  {
    importance[i].first = i;
    importance[i].second = abs(confidence[i].nom - (confidence[i].denom - confidence[i].nom));
  }
  std::sort(importance.begin(), importance.end(), sort_pred() );

  std::vector<int> param_rewards;

  param_rewards.resize(confidence.size());
  for(int i=1; i<importance.size(); i++)
  { //straight order, not proportional to difference
    param_rewards[importance[i].first] = i;
  }
  param_rewards[0]=0;
  //add rewards to MDP model
  assignActionRewards(param_rewards);
}

std::vector<unsigned> MDP::brute_force_strategySynthesis()
{

}

std::vector<unsigned> MDP::synthStrategy()
{
std::vector<unsigned> strategy;
 paramImportance();
  //do strategy synthesis (via prismgames?)

  return strategy;
}


