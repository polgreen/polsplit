#include <vector>
#include <iostream>
#include <utility>
#include "src/fraction.h"
#include "src/MC.h"
#include "src/MDP.h"

/*
MDP get_MDP()
{
  MDP::MDP_statet s0, s1, s2, s3;
  transitiont t1, t2;
  std::pair<fractiont,unsigned> p1;
  MDP::action action;
  MDP model;
  model.modelparams.resize(2);
  model.modelparams[0].one();
  model.modelparams[1].nom = 75;
  model.modelparams[1].denom = 100;

  s0 = {};
  s0.init = true;
  t1.type=FUNCTION;
  p1.first.one();
  p1.second = 1;
  t1.params.push_back(p1);
  t1.successor = 1;
  action.push_back(t1);
  t2.type = REMAINDER;
  t2.successor = 0;
  action.push_back(t2);
  s0.actions.push_back(action);
  action.clear();
  t1.type = CONST;
  t1.prob.one();
  t1.successor = 2;
  action.push_back(t1);
  s0.actions.push_back(action);


  model.MDPstates.push_back(s0);

  s1.init = false;
  t1.type=FUNCTION;
  t1.successor = 2;
  t2.successor = 3;
  action.clear();
  action.push_back(t1);
  action.push_back(t2);
  s1.actions.push_back(action);
  model.MDPstates.push_back(s1);

  //s2
  s2.init=false;
  t1.type = CONST;
  t1.prob.one();
  t1.successor = 2;
  action.clear();
  action.push_back(t1);
  s2.actions.push_back(action);
  model.MDPstates.push_back(s2);

  //s3
  s3.init=false;
  t1.prob.one();
  t1.successor = 3;
  action.clear();
  action.push_back(t1);
  s3.actions.push_back(action);
  model.MDPstates.push_back(s3);

model.success = 2;
model.add_IDs();

  return model;
}*/


MDP get_MDP()
{
  MDP::MDP_statet s0, s1, s2, s3;
  transitiont t1, t2;
  std::pair<fractiont,unsigned> p1;
  MDP model;
  std::vector<transitiont> action1;

  model.modelparams.resize(2);
  model.modelparams[0].one();
  model.modelparams[1].nom = 50;
  model.modelparams[1].denom = 100;

  //s0
  s0 = {};
  s0.init = true;
  t1.type=FUNCTION;
  p1.first.one();
  p1.second = 1;
  t1.params.push_back(p1);
  t1.successor = 1;
  action1.push_back(t1);
//  s0.transitions.push_back(t1);
  t2.type = REMAINDER;
  t2.successor = 0;
  action1.push_back(t2);
 // s0.transitions.push_back(t2);
  s0.actions.push_back(action1);
  model.MDPstates.push_back(s0);

  action1.clear();
  //s1
  s1.init = false;
  t1.successor = 2;
  t2.successor = 3;
  action1.push_back(t1);
  action1.push_back(t2);
 // s1.transitions.push_back(t1);
  //s1.transitions.push_back(t2);
  s1.actions.push_back(action1);
  model.MDPstates.push_back(s1);

  action1.clear();
  //s2
  s2.init=false;
  t1.type = CONST;
  t1.prob.one();
  t1.successor = 2;
  action1.push_back(t1);
  s2.actions.push_back(action1);
 // s2.transitions.push_back(t1);
  model.MDPstates.push_back(s2);

  action1.clear();
  //s3
  s3.init=false;
  t1.prob.one();
  t1.successor = 3;
  action1.push_back(t1);
  s3.actions.push_back(action1);
 // s3.transitions.push_back(t1);
  model.MDPstates.push_back(s3);

model.success = 2;
model.add_IDs();

return model;
}

MC get_MC()
{
	statet s0, s1, s2, s3;
	transitiont t1, t2;
	std::pair<fractiont,unsigned> p1;
 	MC model;

 	model.modelparams.resize(2);
 	model.modelparams[0].one();
 	model.modelparams[1].nom = 50;
 	model.modelparams[1].denom = 100;

 	//s0
 	s0 = {};
 	s0.init = true;
 	t1.type=FUNCTION;
 	p1.first.one();
 	p1.second = 1;
 	t1.params.push_back(p1);
 	t1.successor = 1;
 	s0.transitions.push_back(t1);
 	t2.type = REMAINDER;
 	t2.successor = 0;
 	s0.transitions.push_back(t2);
 	model.states.push_back(s0);

 	//s1
 	s1.init = false;
 	t1.successor = 2;
 	t2.successor = 3;
 	s1.transitions.push_back(t1);
 	s1.transitions.push_back(t2);
 	model.states.push_back(s1);

 	//s2
 	s2.init=false;
 	t1.type = CONST;
 	t1.prob.one();
 	t1.successor = 2;
 	s2.transitions.push_back(t1);
 	model.states.push_back(s2);

 	//s3
 	s3.init=false;
 	t1.prob.one();
 	t1.successor = 3;
 	s3.transitions.push_back(t1);
 	model.states.push_back(s3);

model.success = 2;
model.add_IDs();
return model;
}



