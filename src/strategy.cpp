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

struct sort_pred {
    bool operator()(const std::pair<int,int> &left, const std::pair<int,int> &right) {
        return left.second > right.second;
    }
};

void MDP::assignImportanceRewards()
{
  //difference between (confidence) and (1-confidence) for each parameter
  std::vector<std::pair<int, int>> importance;
  importance.resize(confidence.size());
  for(int i=0; i<confidence.size(); i++)
  {
    importance[i].first = i;
    importance[i].second = abs(confidence[i].nom - (confidence[i].denom - confidence[i].nom));
  }
  std::sort(importance.begin(), importance.end(), sort_pred() );

  std::vector<int> param_rewards;
  param_rewards.resize(confidence.size());
  for(int i=0; i<importance.size(); i++)
  { //straight order, not proportional to difference
    param_rewards[importance[i].first] = i;
  }

  //add rewards to MDP model


}



std::vector<unsigned> MDP::synthStrategy()
{
std::vector<unsigned> strategy;
  assignImportanceRewards();
  //do strategy synthesis (via prismgames?)

  return strategy;
}


