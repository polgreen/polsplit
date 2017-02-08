#include <vector>
#include "model.h"
#include <random>
#include <iostream>
#include <cassert>
#include "fraction.h"

double Generate(const double from, const double to)
{
	double result;
    std::random_device rd;

   result =  std::bind(
        std::uniform_real_distribution<>{from, to},
        std::default_random_engine{ rd() })();
     //   std::cout<<"from:"<<from<<" to:"<<to<<" random:" <<result<<"\n";


        return result;
}

tracet MC::gettrace(unsigned length)
{	
	tracet trace;
	std::vector<std::vector<unsigned> > count;
	statet state = get_init_state();
	bool gotnext = false;
	unsigned next=0;
	trace.push_back(state);
	
	while (trace.size() < length)
	{
        fractiont sum;
        sum.zero();
       //get total weighting of outgoing transitions
        for (const auto& t :state.transitions)
        { //std::cout<<"debug 3 \n"; 
         sum = sum + weighting(t,state);}

		//std::uniform_int_distribution<unsigned> distribution(0,100);
        fractiont random;
        random.nom = Generate(0, 100);
        random.denom = 100;
        fractiont mass;
        fractiont subtraction;
        mass.zero();
       
        for(const auto& t : state.transitions)
         {
         	mass = mass + weighting(t,state);
         	subtraction = mass - random;
           if((subtraction)>0)
            { next = t.successor; gotnext=true;break;}   
         }

         if(gotnext==false){std::cout<<"ERROR NOT FOUND";
     		throw std::exception();}
         if(state.ID>=states.size()|| next>=states.size())
         	{std::cout<<"error in get trace"; throw std::exception();}
        	 
        state = states[next];
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

void MC::get_trace_counts(tracet &trace)
{
	for(unsigned i=0; i<trace.size()-1; i++)
	{
		if(states[trace[i].ID].transitions.size()==0){std::cout<<"Error no transitions on state"<<trace[i].ID<<"\n";
			throw std::exception();}
			
		for(unsigned t=0; t<states[trace[i].ID].transitions.size(); t++)
		{
			if(states[trace[i].ID].transitions[t].successor==trace[i+1].ID)
				{
					states[trace[i].ID].transitions[t].count++;
					states[trace[i].ID].input++;
				}
		}

	}
}

void MC::get_data(unsigned length)
{
  tracet T = gettrace(length);
  get_trace_counts(T);
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