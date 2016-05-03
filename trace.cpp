#include <vector>
#include "model.h"
#include <random>
#include <iostream>
#include <cassert>
#include "fraction.h"


trace_pair gettrace(std::default_random_engine &generator, MC model, unsigned length)
{	

	trace_pair result;
	tracet trace;
	std::vector<std::vector<unsigned> > count;
	statet state = model.get_init_state();
	unsigned next, i, product;
	

	std::cout<<"debug 1 \n";
	result.second.resize(model.states.size());

	for(auto & c: result.second)
	{
	 	c.resize(model.states.size());
		//std::fill(c.begin(),c.end(),0);
	}
	std::cout<<"debug 2 \n";
	//trace.push_back(state);
	

	while (trace.size() < length)
	{
        fractiont sum;
        sum.zero();
       //get total weighting of outgoing transitions
        for (const auto& t :state.transitions)
        { std::cout<<"debug 3 \n"; 
         sum = sum.add(model.weighting(t,state));}  

		std::uniform_int_distribution<unsigned> distribution(0,sum.nom-1);
        fractiont random;
        random.nom = distribution(generator);
        random.denom = sum.nom;
        fractiont mass;
        mass.zero();
        
        for(const auto& t : state.transitions)
         {
         	mass = mass.add(model.weighting(t,state));
           if(mass.subtract(random).nom>0)
            { next = t.successor; break;}   
         }

        result.second[state.ID][next]++;	 
        state = model.states[next];
		trace.push_back(state);

	}
	result.first = trace;
	return result;
}





void printtrace(trace_pair trace)
{
	std::cout<<"\n";
    for(const auto &s: trace.first) 
    {
        printstate(s);
    }
    std::cout<<"\n";
}