/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "fraction.h"
#include "model.h"
#include "model_mdp.h"

MDP MDP_cmdvars::get_MDP_instance(int option) {
    MDP model;
    model.model_num = option;
    if (option == 0) {

        MDP::statet_a s0, s1, s2, s3;
        MDP::action action1, action2;
        transitiont t1, t2, t3;
        s0.ID = 0;
        s1.ID = 1;
        s2.ID = 2;
        s3.ID = 3;

        model.modelparams.resize(2);
        model.modelparams[0].one();
        model.modelparams[1].nom = rud_param_values[0];
        model.modelparams[1].denom = 100;

        model.param_upper_bounds.resize(2);
        model.param_lower_bounds.resize(2);
        model.param_upper_bounds[0] = 1.0;
        model.param_upper_bounds[1] = 1.0;
        model.param_lower_bounds[0] = 1.0;
        model.param_lower_bounds[1] = 0.0;

        s0.init = true;
        t1.type = CONST;
        t1.prob.one();
        t1.successor = s1.ID;
        action1.first.push_back(t1);
        s0.actions.push_back(action1);
        model.states.push_back(s0);
        action1.first.clear();

        s2.init = false;
        t1.type = CONST;
        t1.prob.one();
        t1.successor = s2.ID;
        action1.first.push_back(t1);
        s2.actions.push_back(action1);
        model.states.push_back(s2);
        action1.first.clear();

        s3.init = false;
        t1.type = CONST;
        t1.prob.one();
        t1.successor = s3.ID;
        action1.first.push_back(t1);
        s3.actions.push_back(action1);
        model.states.push_back(s3);
        action1.first.clear();


        s1.init = false;
        t1.type = FUNCTION;
        std::pair<fractiont, unsigned> p1;
        p1.first.one();
        p1.second = 1;
        t1.params.push_back(p1);
        t1.successor = s2.ID;
        action1.first.push_back(t1);
        t2.type = REMAINDER;
        t2.successor = s3.ID;
        action1.first.push_back(t2);

        t3.type = CONST;
        t3.prob.nom = 70;
        t3.prob.denom = 100;
        t3.successor = s0.ID;
        action2.first.push_back(t3);
        t3.type = REMAINDER;
        t3.prob.nom = 0;
        t3.prob.denom = 0;
        t3.successor = s3.ID;
        action2.first.push_back(t3);
        s1.actions.push_back(action1);
        s1.actions.push_back(action2);
        model.states.push_back(s1);
        action1.first.clear();
        action2.first.clear();


        model.success = s3.ID;


    } else if (option == 1) {
        MDP::statet_a s0, s1, s2, s3, s4;
        MDP::action action1, action2;
        transitiont t1, t2, t3;
        s0.ID = 0;
        s1.ID = 1;
        s2.ID = 2;
        s3.ID = 3;
        s4.ID = 4;



        model.modelparams.resize(3);
        model.modelparams[0].one();
        model.modelparams[1].nom = rud_param_values[0];
        model.modelparams[1].denom = 100;
        model.modelparams[2].nom = rud_param_values[1];
        model.modelparams[2].denom = 100;

        model.param_upper_bounds.resize(3);
        model.param_lower_bounds.resize(3);
        model.param_upper_bounds[0] = 1.0;
        model.param_upper_bounds[1] = 0.75;
        model.param_upper_bounds[2] = 0.9;
        model.param_lower_bounds[0] = 1.0;
        model.param_lower_bounds[1] = 0.0;
        model.param_lower_bounds[2] = 0.0;

        s0.init = true;
        t1.type = CONST;
        t1.prob.one();
        t1.successor = s2.ID;
        action1.first.push_back(t1);
        s0.actions.push_back(action1);
        action1.first.clear();

        t1.type = CONST;
        t1.prob.one();
        t1.successor = s1.ID;
        action1.first.push_back(t1);
        s0.actions.push_back(action1);
        action1.first.clear();

        t1.type = FUNCTION;
        std::pair<fractiont, unsigned> p1;
        p1.first.one();
        p1.second = 1;
        t1.params.push_back(p1);
        t1.successor = s2.ID;
        action1.first.push_back(t1);
        t2.type = CONST;
        t2.prob.nom = 25;
        t2.prob.denom = 100;
        t2.successor = s0.ID;
        action1.first.push_back(t2);
        t3.type = REMAINDER;
        t3.successor = s3.ID;
        action1.first.push_back(t3);
        s0.actions.push_back(action1);
        action1.first.clear();

        t1.type = CONST;
        t1.params.clear();
        t1.prob.nom = 60;
        t1.prob.denom = 100;
        t1.successor = s2.ID;
        action1.first.push_back(t1);
        t1.type = REMAINDER;
        t1.prob.nom = 0;
        t1.prob.denom = 0;
        t1.successor = s4.ID;
        action1.first.push_back(t1);
        s0.actions.push_back(action1);
        action1.first.clear();
        model.states.push_back(s0);

        s1.init = false;
        t1.type = FUNCTION;
        p1.first.one();
        p1.second = 2;
        t1.params.push_back(p1);
        t1.successor = s1.ID;
        action1.first.push_back(t1);
        t2.type = CONST;
        t2.prob.nom = 10;
        t2.prob.denom = 100;
        t2.successor = s0.ID;
        action1.first.push_back(t2);
        t3.type = REMAINDER;
        t3.successor = s2.ID;
        action1.first.push_back(t3);

        t3.type = CONST;
        t3.prob.one();
        t3.successor = s2.ID;
        action2.first.push_back(t3);

        s1.actions.push_back(action1);
        s1.actions.push_back(action2);
        model.states.push_back(s1);
        action1.first.clear();
        action2.first.clear();


        s2.init = false;
        t1.type = CONST;
        t1.prob.one();
        t1.successor = s0.ID;
        action1.first.push_back(t1);
        s2.actions.push_back(action1);
        model.states.push_back(s2);
        action1.first.clear();

        s3.init = false;
        t1.type = CONST;
        t1.prob.one();
        t1.successor = s3.ID;
        action1.first.push_back(t1);
        s3.actions.push_back(action1);
        model.states.push_back(s3);
        action1.first.clear();

        s4.init = false;
        t1.type = CONST;
        t1.prob.one();
        t1.successor = s4.ID;
        action1.first.push_back(t1);
        s4.actions.push_back(action1);
        model.states.push_back(s4);
        action1.first.clear();

        model.success = s2.ID;


    }

    return model;
}