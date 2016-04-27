#include <vector>
#include "model.h"
#include <random>
#include <iostream>
#include <cassert>

unsigned weighting(transitiont t, std::vector<unsigned> params)
{
	unsigned sum=0;

	switch(t.type)
	{
		case CONST: return t.prob; break;
		case FUNCTION:
	    for(unsigned index=0; index<t.params.size(); index++)
         	  {sum = sum + t.params.at(index) * params.at(index);}
          return sum; break;
        default:;
    }
       
}


//instead of probabilities, we give weightings for loss and notloss, and use and not use. 
//Probability of a loss = loss/(loss + notloss)

MC get_ZeroConfMC(unsigned probes, unsigned lossWT, unsigned notlossWT, unsigned useWT, unsigned notuseWT)
{
// from http://www.prismmodelchecker.org/papers/allerton10.pdf
 statet state;
 transitiont t1, t2;
 MC Model;

//initial state
 state.ID=0;
 t1.type=CONST;
 t1.prob=useWT;
 t1.successor=1;
 state.transitions.push_back(t1);
 t2.type=CONST;
 t2.prob=notuseWT;
 t2.successor=probes+2;
 state.transitions.push_back(t2);
 state.init=true;
 Model.states.push_back(state);

//probes are states s1,...,sN
 for(unsigned i=0; i<probes; i++)
 {
 	state = {}; t1 = {}; t2 = {};
 	state.ID = i+1;
 	t1.type=CONST;
 	t1.prob=lossWT;
 	t1.successor=i+2;
 	state.transitions.push_back(t1);
 	t2.type=CONST;
 	t2.prob=notlossWT;
 	t2.successor=i;
 	state.transitions.push_back(t2);
 	state.init=false;
 	Model.states.push_back(state);


 }

 state = {}; 
 t1 = {}; 
 t2 = {};
 state.ID = probes+1;
 t1.type=CONST;
 t1.prob=10;
 t1.successor=probes+1;
 state.transitions.push_back(t1);
 state.init=false;
 state.label=0;//fail
 Model.states.push_back(state);


 state = {}; t1 = {}; t2 = {};
 state.ID = probes+2;
 t1.type=CONST;
 t1.prob=10;
 t1.successor=probes+2;
 state.transitions.push_back(t1);
 state.init=false;
 state.label=1; //transmit message
 Model.states.push_back(state);

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
	unsigned next, i, product;
	//std::cout<<"initial state ID: "<<state.ID<<"\n";

	trace.push_back(state);
	while (trace.size() < length)
	{
        unsigned sum = 0;
       //get total weighting of outgoing transitions
        for (const auto& t :state.transitions)
        {   
         sum = sum + weighting(t, model.params);
        }  

		std::uniform_int_distribution<unsigned> distribution(0,sum-1);
        unsigned random = distribution(generator);
        unsigned mass =0;
        std::cout<<random<<" ";
        for(const auto& t : state.transitions)
         {
         	mass = mass + weighting(t, model.params);
           if(mass > random)
            { next = t.successor; break;}   
         	}

        state = get_state_ID(next, model);
		trace.push_back(state);
		}
	return trace;
}


void printtrace(tracet trace)
{
	std::cout<<"\n";
    for(const auto &s: trace) 
    {
        printstate(s);
    }
    std::cout<<"\n";
}

void outputMC (MC model)
{
	std::cout<<"\nMARKOV CHAIN: \n";
	std::cout<<"parameters: ";
	for(const auto &p: model.params)
		{std::cout<<p << " ";}
	std::cout<"\nSTATES \n";
	for (const auto &s : model.states)
	{
		std::cout<<"S"<<s.ID <<" "<<s.label<<": \n";
		for(const auto & t: s.transitions)
		{
			std::cout<<"transition "<<t.type<<" to S";
			std::cout<<t.successor<<" weighting";
			std::cout<<weighting(t, model.params)<<"\n";
		}
	}			

}

