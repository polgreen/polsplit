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
	if(states.size()==0)
		{std::cout<<"error in MC::check(), no states found \n";
		throw std::exception();}

	for(const auto &s: states)
	{
		if(s.ID>=states.size()){std::cout<<"StateIDs not properly assigned";
		throw std::exception();}
		state_sum.zero();
		for(const auto &t: s.transitions)
		{
			weight = weighting(t,s);
			if(weight.nom<0 || weight.denom<0)
				{std::cout<<"ERROR: transition S"<<s.ID<<"->S"<<t.successor<<" probability less than 0 \n";
				throw std::exception();}
			state_sum = weight + state_sum;
			if(state_sum.nom > state_sum.denom)
				{std::cout<<"ERROR: transitions from S"<<s.ID<<" sum to more than 1 \n";
				throw std::exception();}
			if(t.type==FUNCTION)
				{ for(const auto &p: t.params)
					{
						param_sum = p.first + param_sum;
						if(param_sum.nom>param_sum.denom){std::cout<<"ERROR: parameter multipliers >1";
							throw std::exception();}
					}
				}	
		}
	}
}


std::vector<std::vector<statet>> MC::get_parameterised_states()
{
	if(states.size()==0){
		std::cout<<"no model states found\n";
		throw std::exception();}
	std::vector<std::vector<statet>> result;
	bool found=false;
	for(const auto &s:states)
	{
		for(const auto &t: s.transitions)
		{
			if(t.type==FUNCTION && t.params.size()==1)
				{	
					result[t.params[0].second].push_back(s);
					found=true;
				}
		}
		if(found==false){std::cout<<"error in get_parameterised_states: no parameterised states found \n";
		throw std::exception();}
	}
return result;

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
         	  	prod = t.params[index].first*modelparams[t.params[index].second];
         	  	sum = prod + sum;
         	  }
          return sum; break;
         case REMAINDER: return remainderWeight(s) ;break; 
        default: std::cout<<"error, state"<<s.ID<<"type unknown \n";
        		throw std::exception(); 
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
	 	{std::cout<<"error, 2 transitions of type REMAINDER found on S"<<s.ID<<"\n";
		 throw std::exception();}
	 	else{sum_state = sum_state + weighting(t, s);}
	 }
	result.one();
	if(sum_state.nom>sum_state.denom || sum_state.nom<=0)
		{std::cout<<"error, invalid state. \n";
		throw std::exception();}
	result = result-sum_state;
	return result;
}



statet MC::get_init_state()
{
	bool found;
	for (const auto& s: states)
	{
		if(s.init==true && found==false)
		{return s;}
		else if(s.init==true && found==true)
			{std::cout<<"error in get_init_state: found 2 initial states \n";
			throw std::exception();}
	}
 //if we get here throw exception
	std::cout<<"error in get_init_state: no initial state found \n";
		 throw std::exception();

}


void printstate(statet s)
{
	std::cout<<s.ID <<", ";
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

