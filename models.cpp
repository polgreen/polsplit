#include <vector>
#include <iostream>
#include <utility>
#include "src/fraction.h"
#include "src/MC.h"
#include "src/MDP.h"
#include "src/model_param.h"


MDP get_MDP_four()
{
    MDP model;
    model.num_states(5);
    model.modelparams.resize(3);
    model.modelparams[0].one();
    model.modelparams[1].nom = PARAM1;
    model.modelparams[1].denom = 100;
    model.modelparams[2].nom = PARAM2;
    model.modelparams[2].denom = 100;

    double param1[] =
    { 0, 1, 0 };
    double param2[] =
    { 0, 0, 1 };

    model.add_param_transition(0, 0, 2, param1);
    model.add_const_transition(0, 0, 0, 0.25);
    model.add_remainder_transition(0, 0, 3);
    model.add_const_transition(0, 1, 1, 1);
    model.add_const_transition(0,2,2,1);
    model.add_const_transition(0,3,2,0.6);
    model.add_remainder_transition(0,3,4);

    model.add_param_transition(1, 0, 1, param2);
    model.add_const_transition(1, 0, 0, 0.1);
    model.add_remainder_transition(1, 0, 2);
    model.add_const_transition(1,1,2,1);

     model.add_const_transition(2, 0, 0, 1);
     model.add_const_transition(3, 0, 3, 1);
     model.add_const_transition(4,0,4,1);

     model.make_state_init(0);
     model.success=2;
     model.add_IDs();
     model.check();

     return model;
}

MDP get_MDP_0()
{

  MDP model;
  model.num_states(4);

  model.modelparams.resize(2);
  model.modelparams[0].one();
  model.modelparams[1].nom = PARAM1;
  model.modelparams[1].denom = 100;

  std::cout << "test" << std::endl;

  model.add_const_transition(0, 0, 1, 1); //breaks PRISM
  double param_multipliers[] =
  { 0, 1 };
  model.add_param_transition(1, 0, 2, param_multipliers);
  model.add_remainder_transition(1, 0, 3);
  model.add_const_transition(1, 1, 0, 0.7);
  model.add_remainder_transition(1, 1, 3);
  model.add_const_transition(2, 0, 0, 1);
  model.add_const_transition(3, 0, 3, 1);
  model.make_state_init(0);
  model.success = 3;

  model.add_IDs();
  std::cout << "checking model\n";
  model.check();

  return model;
}

MDP get_MDP_two()
{
  //page 14 of lecture 13 PMC
  MDP model;
  model.num_states(4);
  model.modelparams.resize(3);
  model.modelparams[0].one();
  model.modelparams[1].nom = PARAM1;
  model.modelparams[1].denom = 100;
  model.modelparams[2].nom = PARAM2;
  model.modelparams[2].denom = 100;

  model.param_upper_bounds.resize(3);
  model.param_lower_bounds.resize(3);
  model.param_upper_bounds[0] = 1.0;
  model.param_upper_bounds[1] = 0.75;
  model.param_upper_bounds[2] = 0.9;
  model.param_lower_bounds[0] = 1.0;
  model.param_lower_bounds[1] = 0.0;
  model.param_lower_bounds[2] = 0.0;

  double param_multipliers[] =
  { 0, 1, 0 };
  model.add_const_transition(0, 1, 1, 1);
  model.add_param_transition(0, 0, 2, param_multipliers);
  model.add_const_transition(0, 0, 0, 0.25);
  model.add_remainder_transition(0, 0, 3);

  double param_multipliers2[] =
  { 0, 0, 1 };
  model.add_param_transition(1, 0, 1, param_multipliers2);
  model.add_const_transition(1, 0, 0, 0.1);
  model.add_remainder_transition(1, 0, 2);

  model.add_const_transition(2, 0, 0, 1);
  model.add_const_transition(3, 0, 3, 1);
  //model.add_const_transition(3,1,3,1);

  model.make_state_init(0);

  model.success=2;
  model.add_IDs();
  model.check();

  return model;
}

MDP get_MDP_three()
{
  MDP model;
  model.num_states(5);
  model.modelparams.resize(3);
  model.modelparams[0].one();
  model.modelparams[1].nom = PARAM1;
  model.modelparams[1].denom = 100;
  model.modelparams[2].nom = PARAM2;
  model.modelparams[2].denom = 100;

  model.param_upper_bounds.resize(3);
  model.param_lower_bounds.resize(3);
  model.param_upper_bounds[0] = 1.0;
  model.param_upper_bounds[1] = 0.75;
  model.param_upper_bounds[2] = 0.9;
  model.param_lower_bounds[0] = 1.0;
  model.param_lower_bounds[1] = 0.0;
  model.param_lower_bounds[2] = 0.0;

  double param1[] =
  { 0, 1, 0 };
  double param2[] =
  { 0, 0, 1 };


  model.add_param_transition(0, 2, 2, param1);
  model.add_const_transition(0, 2, 0, 0.25);
  model.add_remainder_transition(0, 2, 3);
  model.add_const_transition(0, 1, 1, 1);
  model.add_const_transition(0,0,2,1);
  model.add_const_transition(0,3,2,0.6);
  model.add_remainder_transition(0,3,4);

  model.add_param_transition(1, 0, 1, param2);
  model.add_const_transition(1, 0, 0, 0.1);
  model.add_remainder_transition(1, 0, 2);
  model.add_const_transition(1,1,2,1);

   model.add_const_transition(2, 0, 0, 1);
   model.add_const_transition(3, 0, 3, 1);
   model.add_const_transition(4,0,4,1);

   model.make_state_init(0);
   model.success=2;
   model.add_IDs();
   model.check();

   return model;
}

MDP get_MDP_one()
{
  MDP model;
  model.num_states(5);
  model.modelparams.resize(3);
  model.modelparams[0].one();
  model.modelparams[1].nom = PARAM1;
  model.modelparams[1].denom = 100;
  model.modelparams[2].nom = PARAM2;
  model.modelparams[2].denom = 100;

  model.param_upper_bounds.resize(3);
  model.param_lower_bounds.resize(3);
  model.param_upper_bounds[0] = 1.0;
  model.param_upper_bounds[1] = 0.75;
  model.param_upper_bounds[2] = 0.9;
  model.param_lower_bounds[0] = 1.0;
  model.param_lower_bounds[1] = 0.0;
  model.param_lower_bounds[2] = 0.0;

  double param1[] =
  { 0, 1, 0 };
  double param2[] =
  { 0, 0, 1 };

  model.add_param_transition(0, 0, 2, param1);
  model.add_const_transition(0, 0, 0, 0.25);
  model.add_remainder_transition(0, 0, 3);
  model.add_const_transition(0, 1, 1, 1);
  model.add_const_transition(0,2,2,1);
  model.add_const_transition(0,3,2,0.6);
  model.add_remainder_transition(0,3,4);

  model.add_param_transition(1, 0, 1, param2);
  model.add_const_transition(1, 0, 0, 0.1);
  model.add_remainder_transition(1, 0, 2);

   model.add_const_transition(2, 0, 0, 1);
   model.add_const_transition(3, 0, 3, 1);
   model.add_const_transition(4,0,4,1);

   model.make_state_init(0);
   model.success=2;
   model.add_IDs();
   model.check();

   return model;
}

MDP get_MDP(int m)
{
  switch (m)
  {
    case 0:
      std::cout<<"MDP 0"<<std::endl;
      return get_MDP_0();
    case 1:
      std::cout<<"MDP 1"<<std::endl;
      return get_MDP_one();
    case 2:
      std::cout<<"MDP 2"<<std::endl;
      return get_MDP_two();
    case 3:
      std::cout<<"MDP 3"<<std::endl;
      return get_MDP_three();
    default:
      std::cout<<"MDP 0"<<std::endl;
      return get_MDP_0();
  }

}

MC get_MC()
{
  statet s0, s1, s2, s3;
  transitiont t1, t2;
  std::pair<fractiont, unsigned> p1;
  MC model;

  model.modelparams.resize(2);
  model.modelparams[0].one();
  model.modelparams[1].nom = PARAM1;
  model.modelparams[1].denom = 100;

  //s0
  s0 =
  {};
  s0.init = true;
  t1.type = FUNCTION;
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
  s2.init = false;
  t1.type = CONST;
  t1.prob.one();
  t1.successor = 2;
  s2.transitions.push_back(t1);
  model.states.push_back(s2);

  //s3
  s3.init = false;
  t1.prob.one();
  t1.successor = 3;
  s3.transitions.push_back(t1);
  model.states.push_back(s3);

  model.success = 2;
  model.add_IDs();
  return model;
}

