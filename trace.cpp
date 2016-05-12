#include <vector>
#include "model.h"
#include <random>
#include <iostream>
#include <cassert>
#include "fraction.h"



tracet gettrace(std::default_random_engine &generator, MC model, unsigned length)
{	
	tracet trace;
	std::vector<std::vector<unsigned> > count;
	statet state = model.get_init_state();
	bool gotnext;
	unsigned next, i, product;
	trace.push_back(state);
	
	while (trace.size() < length)
	{
        fractiont sum;
        sum.zero();
       //get total weighting of outgoing transitions
        for (const auto& t :state.transitions)
        { //std::cout<<"debug 3 \n"; 
         sum = sum + model.weighting(t,state);}  

		std::uniform_int_distribution<unsigned> distribution(0,sum.nom-1);
        fractiont random;
        random.nom = distribution(generator);
        random.denom = sum.nom;
        fractiont mass;
        mass.zero();
        
        for(const auto& t : state.transitions)
         {
         	mass = mass + model.weighting(t,state);
           if((mass - random)>0)
            { next = t.successor; gotnext=true;break;}   
         }

         if(gotnext==false){std::cout<<"ERROR NOT FOUND";
     		throw std::exception();}
         if(state.ID>=model.states.size()|| next>=model.states.size())
         	{std::cout<<"error in get trace"; throw std::exception();}
        	 
        state = model.states[next];
		trace.push_back(state);

	}

	return trace;
}

//make this pass by reference
unsigned trace_count(unsigned s1, unsigned s2, tracet t)
{
	unsigned count;
	for(unsigned i=0; i<t.size(); i++)
	{
		if(t[i].ID==s1 && t[i+1].ID==s2)
			{count++;}
		i++;
	}
	return count;
}

void MC::get_trace_counts(tracet trace)
{
	for(unsigned i=0; i<trace.size(); i++)
	{
		if(states[trace[i].ID].transitions.size()==0){std::cout<<"Error no transitions on state"<<trace[i].ID<<"\n";
		throw std::exception();}
		for(unsigned t=0; t<states[trace[i].ID].transitions.size(); t++)
		{
		
			if(states[trace[i].ID].transitions[t].successor==trace[i+1].ID)
			{states[trace[i].ID].transitions[t].count++;}
		}
	}
}

std::vector<fractiont> parameter_distributions(MC model)
{
	std::cout<<"parameter distributions \n";
	std::vector< std::vector< std::pair < statet, unsigned> > > param_states;
	param_states = model.get_parameterised_states();
	std::vector<fractiont> result;

	result.resize(param_states.size());
	for(auto r: result) //initialise vector
		{r.zero();}

	for(unsigned p_index; p_index<param_states.size(); p_index++)
	{
		for(const auto s: param_states[p_index])
		{
			result[p_index]=result[p_index] + s.first.transitions[s.second].count;
			std::cout<<"count is: "<<s.first.transitions[s.second].count;
		}
	}

	for(auto f: result)
	{
		std::cout<<"dist: "<<f.nom<<" "<<f.denom;
		std::cout<<"\n";
	}


return result;
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