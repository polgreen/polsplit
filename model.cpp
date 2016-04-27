#include <vector>
#include "model.h"
#include <random>
#include <iostream>
#include <cassert>


//instead of probabilities, we give weightings for loss and notloss, and use and not use. 
//Probability of a loss = loss/(loss + notloss)

MC get_ZeroConfMC(unsigned probes, unsigned lossWT, unsigned notlossWT, unsigned useWT, unsigned notuseWT)
{
// from http://www.prismmodelchecker.org/papers/allerton10.pdf
 statet state;
 MC Model;

//initial state
 state.ID=0;
 state.successors.push_back(1);
 state.prob.push_back(useWT);
 state.successors.push_back(probes+2);
 state.prob.push_back(notuseWT);
 state.init=true;
 Model.states.push_back(state);
 Model.num=Model.num+1;

//probes are states s1,...,sN
 for(unsigned i=0; i<probes; i++)
 {
 	state = {};
 	state.ID = i+1;
 	state.successors.push_back(i+2);
 	state.prob.push_back(lossWT);
 	state.successors.push_back(i);
 	state.prob.push_back(notlossWT);
 	state.init=false;
 	Model.states.push_back(state);
 	Model.num=Model.num+1;
 }

//final state
state = {};
state.ID=probes+1;
state.successors.push_back(probes+1);
state.prob.push_back(10);
state.label=0; //FAIL
Model.states.push_back(state);
Model.num=Model.num+1;
state={};
state.ID=probes+2;
state.successors.push_back(probes+2);
state.prob.push_back(10);
state.label=1; //SUCCEED
Model.states.push_back(state);
Model.num=Model.num+1;

return Model;
}

statet get_init_state(MC model)
{
	for (const auto& s: model.states)
	{
		if(s.init==true)
		{
			return s;
		}
	}//add error handling, what if we find the ID twice, and what if we don't find it at all
}

statet get_state_ID(unsigned ID, MC model)
{
	for(const auto& s: model.states)
	{
		if(s.ID ==ID)
		{
			return s;
		}
	}
}//add error handling, what if we find the ID twice, and what if we don't find it at all


void printstate(statet s)
{
	std::cout<<s.ID <<", ";
}


tracet gettrace(std::default_random_engine &generator, MC model, unsigned length)
{	
	tracet trace;
	statet state = get_init_state(model);
	unsigned next;
	unsigned i;
	std::cout<<"initial state ID: "<<state.ID<<"\n";

	trace.push_back(state);
	while (trace.size() < length)
	{
     	i=0;
        unsigned sum = 0;
       
        for (const auto& p :state.prob)
        {   
            sum = sum + p;
        }  
       std::cout<<"sum "<<sum << "\n";
		std::uniform_int_distribution<unsigned> distribution(0,sum-1);
        unsigned random = distribution(generator);
        unsigned mass =0;
        std::cout<<random<<" ";
        for(const auto& p : state.prob)
         {
          mass = mass + p;
          std::cout<<"mass: "<<mass<<"\n";
          if(mass > random)
           { 
          	next = state.successors.at(i); 
          	std::cout<<"state ID: "<<next<<"\n";break;
           }   
         i = i+1;
         }
        
        state = get_state_ID(next, model);
		trace.push_back(state);
	}
	return trace;
}


void printtrace(tracet trace)
{
    for(const auto &s: trace) 
    {
        printstate(s);
    }
    std::cout<<"\n";
}