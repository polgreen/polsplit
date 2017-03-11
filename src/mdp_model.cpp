/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "fraction.h"
#include "model.h"
#include "model_mdp.h"

MDP get_MDP_instance() {

    MDP::statet_a s0, s1, s2, s3;
    std::vector<transitiont> action1, action2;
    transitiont t1, t2, t3, t4;
    MDP model;
    s0.ID = 0;
    s1.ID = 1;
    s2.ID = 2;
    s3.ID = 3;

    model.modelparams.resize(2);
    model.modelparams[0].one();
    model.modelparams[1].nom = 73;
    model.modelparams[1].denom = 100;

    s0.init = true;
    t1.type = CONST;
    t1.prob.one();
    t1.successor = s1.ID;
    action1.push_back(t1);
    s0.actions.push_back(action1);
    model.states.push_back(s0);
    action1.clear();

    s2.init = false;
    t1.type = CONST;
    t1.prob.one();
    t1.successor = s2.ID;
    action1.push_back(t1);
    s2.actions.push_back(action1);
    model.states.push_back(s2);
    action1.clear();

    s3.init = false;
    t1.type = CONST;
    t1.prob.one();
    t1.successor = s3.ID;
    action1.push_back(t1);
    s3.actions.push_back(action1);
    model.states.push_back(s3);
    action1.clear();
    model.success = s3.ID;

    s1.init = false;
    t1.type = FUNCTION;
    std::pair<fractiont, unsigned> p1;
    p1.first.one();
    p1.second = 1;
    t1.params.push_back(p1);
    t1.successor = s2.ID;
    action1.push_back(t1);
    t2.type = REMAINDER;
    t2.successor = s3.ID;
    action1.push_back(t2);

    t3.type = CONST;
    t3.prob.nom = 70;
    t3.prob.denom = 100;
    t3.successor = s0.ID;
    action2.push_back(t3);
    t3.type = REMAINDER;
    t3.prob.nom = 0;
    t3.prob.denom = 0;
    t3.successor = s3.ID;
    action2.push_back(t3);
    s1.actions.push_back(action1);
    s1.actions.push_back(action2);
    model.states.push_back(s1);


    return model;




}