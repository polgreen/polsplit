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
         	  {sum = sum + t.params[index].first * params[index];}
          return sum; break;
        default:;
    }
       
}


//instead of probabilities, we give weightings for loss and notloss, and use and not use. 
//Probability of a loss = loss/(loss + notloss)



statet MC::get_init_state()
{
	for (const auto& s: states)
	{
		if(s.init==true)
		{
			return s;
		}
	}//add error handling, what if we find the ID twice, and what if we don't find it at all
}


void printstate(statet s)
{
	std::cout<<s.ID <<", ";
}


tracet gettrace(std::default_random_engine &generator, MC model, unsigned length)
{	
	tracet trace;
	statet state = model.get_init_state();
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

        state = model.states[next];
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

void MC::outputMC (std::ostream &out)
{
	out<<"\nMARKOV CHAIN: \n";
	out<<"parameters: ";
	for(const auto &p: params)
		{out<<p << " ";}
	out<<"\nSTATES \n";
	for (const auto &s : states)
	{
		out<<"S"<<s.ID <<": \n";
		for(const auto & t: s.transitions)
		{
			out<<"transition "<<t.type<<" to S";
			out<<t.successor<<" weighting";
			out<<weighting(t, params)<<"\n";
		}
	}			

}

