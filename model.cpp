#include <vector>
#include "model.h"
#include <random>
#include <iostream>
#include <cassert>
#include "fraction.h"



void MC::check()
{
	fractiont weight;
	fractiont state_sum;
	fractiont param_sum;
	for(const auto s: states)
	{
		state_sum.zero();
		for(const auto t: s.transitions)
		{
			weight = weighting(t,s);
			if(weight.nom<0 || weight.denom<0)
				{std::cout<<"ERROR: transition S"<<s.ID<<"->S"<<t.successor<<" probability less than 0 \n";}
			state_sum = state_sum.add(weight);
			if(state_sum.nom > state_sum.denom)
				{std::cout<<"ERROR: transitions from S"<<s.ID<<" sum to more than 1 \n";}
			if(t.type==FUNCTION)
				{ for(const auto &p: t.params)
					{
						param_sum = param_sum.add(p.first);
						if(param_sum.nom>param_sum.denom){std::cout<<"ERROR: parameter multipliers >1";}
					}
				}	
		}
	}
}


fractiont MC::weighting(transitiont t, statet s)
{
	fractiont sum, prod;
	sum.zero();

	switch(t.type)
	{
		case CONST: return t.prob; break;
		case FUNCTION:
	    for(unsigned index=0; index<t.params.size(); index++)
         	  {
         	  	prod = t.params[index].first.multiply(modelparams[t.params[index].second]);
         	  	sum = sum.add(prod);
         	  }
          return sum; break;
         case REMAINDER: return remainderWeight(s) ;break; 
        default:;
    }
       
}



fractiont MC::remainderWeight(statet s)
{
	fractiont sum_state;
	fractiont result;
	bool remainderfound =false;
	sum_state.zero();
	for(const auto & t: s.transitions)
	 {
	 	if(t.type==REMAINDER && remainderfound==false)
	 	{remainderfound=true;}
	 	else if(t.type==REMAINDER && remainderfound==true)
	 	{std::cout<<"error, 2 transitions of type REMAINDER found on S"<<s.ID<<"\n";}
	 	else{sum_state = sum_state.add(weighting(t, s));}
	 }
	result.one();
	result = result.subtract(sum_state);
	return result;
}



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
        fractiont sum;
        sum.zero();
       //get total weighting of outgoing transitions
        for (const auto& t :state.transitions)
        {   
         sum = sum.add(model.weighting(t,state));
        }  

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
	fractiont result;
	out<<"\nMARKOV CHAIN: \n";
	out<<"parameters: ";
	for(auto &p: modelparams)
		{p.output(out);
			out<<" ";}
	out<<"\nSTATES \n";
	for (const auto &s : states)
	{
		out<<"S"<<s.ID <<": \n";
		for(const auto & t: s.transitions)
		{
			out<<"transition "<<t.type<<" to S";
			out<<t.successor<<" weighting: ";
			result = weighting(t, s);
			if(result.nom==0){std::cout<<"ERROR ZERO VALUE RETURNED";}
			result.output(out);
			out<<"\n";
		}
	}			

}

