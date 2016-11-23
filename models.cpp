#include <vector>
#include "model.h"
#include <iostream>
#include <utility>
#include "fraction.h"


void MC::add_IDs ()
{
	for(unsigned i=0; i<states.size(); i++)
	{
		states[i].ID = i;
	}
}


MC get_simpleMC()
{
	statet s0, s1, s2, s3;
	transitiont t1, t2;
	std::pair<fractiont,unsigned> p1;
 	MC model;

 	model.modelparams.resize(2);
 	model.modelparams[0].one();
 	model.modelparams[1].nom = 5;
 	model.modelparams[1].denom = 10;

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
 	t1.successor = 0;
 	s2.transitions.push_back(t1);
 	model.states.push_back(s2);

 	//s3
 	s3.init=false;
 	t1.prob.nom = 1; t1.prob.denom = 2;
 	t1.successor = 0;
 	s3.transitions.push_back(t1);
 	t1.successor = 2;
 	t1.type = REMAINDER;
 	s3.transitions.push_back(t1);
 	model.states.push_back(s3);


model.add_IDs();
return model;
}


MC get_parameterisedMC (unsigned probes, unsigned lossWT, unsigned notlossWT, unsigned useWT, unsigned notuseWT)
{
 //builds a hard coded model
 
 statet state;
 transitiont t1, t2;
 std::pair<fractiont,unsigned> p1;
 MC Model;


 Model.modelparams.resize(3);
 Model.modelparams[0].one();
 Model.modelparams[2].nom = 1;
 Model.modelparams[2].denom =2;
 Model.modelparams[1].nom = 1;
 Model.modelparams[1].denom = 2;

//param [0] = constant;
//initial state
 t1.type=FUNCTION;

 p1.first.nom = 1;
 p1.first.denom = 10;
 p1.second = 1;
 t1.params.push_back(p1);
 p1.second = 2;
 t1.params.push_back(p1);


 t1.successor=1;
 state.transitions.push_back(t1);
 t2.type=REMAINDER;
 t2.successor=probes+2;
 state.transitions.push_back(t2);
 state.init=true;
 Model.states.push_back(state);

//probes are states s1,...,sN
 for(unsigned i=0; i<probes; i++)
 {
 	state = {}; t1 = {}; t2 = {};
 	t1.type=CONST;
 	t1.prob.nom=lossWT;
 	t1.prob.denom = lossWT + notlossWT;
 	t1.successor=i+2;
 	state.transitions.push_back(t1);
 	t2.type=REMAINDER;
 	t2.successor=i;
 	state.transitions.push_back(t2);
 	state.init=false;
 	Model.states.push_back(state);

 }

 state = {}; 
 t1 = {}; 
 t2 = {};
 t1.type=CONST;
 t1.prob.one();
 t1.successor=0;
// t1.successor=probes+1;
 state.transitions.push_back(t1);
 state.init=false;
 state.label=0;//fail
 Model.states.push_back(state);


 state = {}; t1 = {}; t2 = {};
 t1.type=CONST;
 t1.prob.one();
 t1.successor=0;
 //t1.successor=probes+2;
 state.transitions.push_back(t1);
 state.init=false;
 state.label=1; //transmit message
 Model.states.push_back(state);

Model.add_IDs();
return Model;









}