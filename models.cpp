#include <vector>
#include <iostream>
#include <utility>
#include "src/fraction.h"
#include "src/MC.h"
#include "src/MDP.h"
#include "src/model_param.h"




MDP get_printSpooler()
{
  MDP::MDP_statet s0, s1, s2, s3, s4;
  transitiont t1, t2;
  std::pair<fractiont,unsigned> px, py;
  MDP::action action1, action2;
  MDP model;
  model.modelparams.resize(3); //x=1, y=2
  model.modelparams[0].one();
  model.modelparams[1].nom = 75;
  model.modelparams[1].denom = 100;
  model.modelparams[2].nom = 75;
  model.modelparams[2].denom = 100;


  s0 = {};
  s1 = {};
  s2 = {};
  s3 = {};
  s4 = {};
  s0.init = true;
  //action alpha
  t1.type=FUNCTION;
  px.first.one();
  px.second=1;//parameter x
  t1.params.push_back(px);
  t1.successor = 2;
  action1.push_back(t1);
  t2.type = REMAINDER;
  t2.successor = 1;
  action1.push_back(t2);
  s0.actions.push_back(action1);
  //action beta
  py.first.one();
  py.second=2;//parameter y
  t1.params[0]=py;
  t1.successor=1;
  t2.successor=2;
  action2.push_back(t1);
  action2.push_back(t2);
  s0.actions.push_back(action2);

  model.MDPstates.push_back(s0);

  t1.params[0]=px;
  t1.successor=2;
  t2.successor=3;
  action1[0]=t1;
  action1[1]=t2;
  s1.actions.push_back(action1);

  model.MDPstates.push_back(s1);

  t1.params[0]=py;
  t1.successor=1;
  t2.successor=4;
  action1[0]=t1;
  action1[1]=t2;
  s2.actions.push_back(action1);

  model.MDPstates.push_back(s2);

  t1.type=CONST;
  t1.prob.one();
  t1.successor=3;
  action1.clear();
  action1.push_back(t1);
  s3.actions.push_back(action1);

  model.MDPstates.push_back(s3);


  action1[0].successor=4;
  s4.actions.push_back(action1);

  model.MDPstates.push_back(s4);

model.success = 3;
model.add_IDs();

  return model;
}


MDP get_MDP()
{
  MDP::MDP_statet s0, s1, s2, s3;
  transitiont t1, t2;
  std::pair<fractiont,unsigned> p1;
  MDP model;
  std::vector<transitiont> action1, action2;

  model.modelparams.resize(2);
  model.modelparams[0].one();
  model.modelparams[1].nom = PARAM1;
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
  action2.push_back(t1);

//  s0.transitions.push_back(t1);
  t2.type = REMAINDER;
  t2.successor = 3;
  action1.push_back(t2);
 // t2.successor = 2;
  action2.push_back(t2);
 // s0.transitions.push_back(t2);
  s0.actions.push_back(action1);
  s0.actions.push_back(action2);
  model.MDPstates.push_back(s0);

  action1.clear();
  action2.clear();
  //s1
  s1.init = false;
  t1.successor = 2;
  t2.successor = 3;
  action1.push_back(t1);
  action1.push_back(t2);
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
 	model.modelparams[1].nom = PARAM1;
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



