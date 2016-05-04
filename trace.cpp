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

         if(gotnext==false){std::cout<<"ERROR NOT FOUND";}
         if(state.ID>=model.states.size()|| next>=model.states.size())
         	{throw std::exception();}
        	 
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

countt MC::get_trace_counts(tracet trace)
{
	countt result;
	std::pair<unsigned, unsigned> p1;

	for(const auto & s: states)
	{
		for(const auto & trans: s.transitions)
		{
		  if(trans.params.size()>1)
		  {
			std::cout<<"ERROR in get_trace_counts: perform state splitting first \n";
			throw std::exception();
		  }
		  if(trans.type==FUNCTION)
			{
			for(unsigned p_index=0; p_index<modelparams.size(); p_index++)
			 {
				if(trans.params[0].second==modelparams[p_index])
				{
				 result[p_index].first += trace_count(s.ID, trans.successor, trace);
				 for(const auto & t2: s.transitions)
				 {
				   if(trans.successor!=t2.successor)
			       {result[p_index].second+=trace_count(s.ID, t2.successor, trace);}
				 }
				}
			}
          }
		}
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