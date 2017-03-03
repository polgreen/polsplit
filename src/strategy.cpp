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
  if(verbose>1)
    std::cout<<"calculate param importance \n";
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
  if(verbose>1)
  {
    std::cout<<"rewards ";
    for(const auto p: param_rewards)
    {
      std::cout<<p<<" ";
    }
    std::cout<<std::endl;
  }

  //add rewards to MDP model
 // assignActionRewards(param_rewards);
}

std::vector<unsigned> MDP::explicitStrategySynth()
{
  //explicitly compute expected counts for every strategy
  std::vector< std::vector<unsigned> > strategies;
  std::vector<int> expected_counts;
  strategies.resize(1);
  //get all strategies
  for(int i=0; i<MDPstates.size(); i++)
  {
    for(auto &s: strategies)
      { s.push_back(0);}
    if(MDPstates[i].actions.size()>1)
    {
      int currentsize = strategies.size();
      for(int j=1; j<MDPstates[i].actions.size(); j++)
      {
        for(int k=0; k<currentsize; k++ )
        {
          strategies.push_back(strategies[k]);
          strategies[currentsize+k*j-1][i]=j;
        }
      }
    }
  }

  if(verbose>1)
  {
    std::cout<<"all possible strategies: \n";
    for(const auto &s: strategies)
    {
      std::cout<<"strategy ";
      for(unsigned i=0; i<s.size(); i++ )
        std::cout<<"s"<<i<<"-a"<<s[i]<<" ";
      std::cout<<"\n";
    }
  }

  //get expected data counts for all
return strategies[0];
}

std::vector<unsigned> MDP::synthStrategy()
{
  std::vector<unsigned> strategy;
  paramImportance();
  //do strategy synthesis (via prismgames?)
 strategy.resize(MDPstates.size());
 explicitStrategySynth();
 assert(0);

 for(auto s: strategy)
   {s=0;}

 if(verbose>1)
 {
   std::cout<<"Memoryless strategy: ";
   for(const auto s: strategy)
     std::cout<<" a"<<s;
   std::cout<<"\n";
 }
  return strategy;
}


