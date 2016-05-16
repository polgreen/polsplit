#include "model.h"
#include <iostream>
#include "fraction.h"

/*

Is it possible to do the state splitting symbolically?? 

Can we find all states, where the probability of transitioning to a successor state
is in the form c1 X1 + c2 X2 + ... cn Xn?

*/

transitiont find_remainder(statet current_state)
{
	bool rem_found;
	transitiont t_remainder, t0;
	rem_found=false;
	if(current_state.transitions.size()==0)
		{std::cout<<"error in find_remainder, no transitions to state "<<current_state.ID<<"\n";
		throw std::exception();}

	for(unsigned t1_index=0; t1_index<current_state.transitions.size(); t1_index++)
	  {	  	
	 	t0 = current_state.transitions[t1_index];
	 	if(t0.type==REMAINDER && rem_found==false)
	 	{t_remainder = t0; rem_found=true;}
	 	else if(t0.type==REMAINDER && rem_found==true)
	 	{std::cout<<"ERROR: found two remainder transitions from state s"<<current_state.ID<<"\n";}
	  }

	  if(rem_found==false && current_state.transitions.size()>1)
	  	{std::cout<<"error in find_remainder, no REMAINDER transition from state s"<<current_state.ID<<"\n";
		throw std::exception();	}

	return t_remainder;
}



MC state_split1(MC model)
{
	bool case1 =false;	
 statet current_state, new_state;
 transitiont t0, t1, t2, t3, t_remainder;
 std::pair<fractiont,unsigned> p1;
 std::vector<transitiont> temp;
 if(model.states.size()==0)
 {	std::cout<<"error in state_split, no states found \n";
	throw std::exception();
 }

 for(unsigned s_index=0; s_index<model.states.size(); s_index++)
	{
	current_state = model.states[s_index];	
	temp = {};
	case1=false;	
	t_remainder = find_remainder(current_state);
	 for(unsigned t_index=0; t_index<current_state.transitions.size(); t_index++)		
	 {
		t0 = current_state.transitions[t_index];

		 if(t0.type==FUNCTION && t0.params.size()>1)
		 	{
		 		case1=true;
		 		//add new state
		 		 for(unsigned i=0; i<t0.params.size(); i++)
		 		   {
		 		   t1 = {};
		 		   t2 = {};
		 		   new_state = {};
			   		t1.type = CONST;
			   		t1.successor = t0.successor;
			   		t1.prob.one();
			   		t1.added = true;
			   		new_state.transitions.push_back(t1);
			   		new_state.ID = model.states.size();
			   		new_state.added = true;
			   		model.states.push_back(new_state);
			   		
			   	 //add transitions from state to new state
			   	 	t2.type = FUNCTION;
					t2.successor = new_state.ID;
					p1=t0.params[i];
					t2.params.push_back(p1);
					t2.added = true;
					model.states[s_index].transitions.push_back(t2);
					//temp.push_back(t2);
					//std::cout<<temp.size();
				  }	
				model.states[s_index].transitions.erase(model.states[s_index].transitions.begin()+t_index);

		 	}
		 	if(t0.type==FUNCTION && t0.params.size()==1)
		 		{std::cout<<"Error in state_split1, state "<<current_state.ID<<" has FUNCTION transition with no parameters\n";

		 		throw std::exception();}
	}

	}
	return model;
}


MC state_split2(MC model)
{
	if(model.states.size()==0)
 	{std::cout<<"error in state_split2, no states found \n";
	throw std::exception();}

	bool case2=false;
	bool rem_found=false;
	statet current_state, new_state;
	transitiont t0, t1, t2, t3, t_remainder;
	std::pair<fractiont,unsigned> p1;
 	std::vector<transitiont> temp;
 	std::vector<statet> temp_states;

 	std::cout<<"MODEL SIZE:  ";
 	std::cout<<model.states.size();
 	std::cout<<"\n";
 	temp_states = {};

 	for(unsigned s_index=0; s_index<model.states.size(); s_index++)
	 {
	 	current_state = model.states[s_index];
	 	temp = {};
	 	case2=false;
	 	t_remainder = find_remainder(current_state);
	 //	std::cout<<"number of transitions: "<<current_state.transitions.size()<<"\n";

	 	for(unsigned t_index=0; t_index<current_state.transitions.size(); t_index++)
	 	{
	 		t0=current_state.transitions[t_index];
	 		if(t0.type==FUNCTION)
	 		{
	 			if(t0.params.size()==1)
	 			{
	 		//		std::cout<<"FUNCTION: params: "<<t0.params.size()<<"\n";
	 				t1={};
	 				t2={};
	 				new_state={};
	 				new_state.ID = model.states.size() + temp_states.size();
	 				new_state.added = true;
	 				t1.type = FUNCTION;
	 				t1.successor = t0.successor;
	 				t1.params.push_back(t0.params[0]);
	 				t1.params[0].first.one(); //transition probability = parameter multiplied by 1
	 				t2.type = REMAINDER;
	 				t2.successor = t_remainder.successor;
	 				t1.added = true;
	 				t2.added = true;
	 				new_state.transitions.push_back(t1);
	 				new_state.transitions.push_back(t2);
	 				temp_states.push_back(new_state);
	 				t3.added = true;
	 				t3.successor = new_state.ID;
	 				t3.type = CONST;
	 				t3.prob = t0.params[0].first;
	 				model.states[s_index].transitions[t_index] = t3;
	 			}
	 			else{std::cout<<"error in state_split2: please apply state split 1 first \n";
	 				throw std::exception();}
	 		}
	 	}
	 }
	 for(const auto & s: temp_states)
	 {
	 	model.states.push_back(s);
	 }

return model;
}
























