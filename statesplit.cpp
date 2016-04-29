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

	  for(unsigned t1_index=0; t1_index<current_state.transitions.size(); t1_index++)
	  {

	  	rem_found=false;
	 	t0 = current_state.transitions[t1_index];
	 	if(t0.type==REMAINDER && rem_found==false)
	 	{t_remainder = t0; rem_found=true;}
	 	else if(t0.type==REMAINDER && rem_found==true)
	 	{std::cout<<"ERROR: found two remainder transitions from state s"<<current_state.ID<<"\n";}
	  }

	  if(rem_found==false&&current_state.transitions.size()>1)
	  	{std::cout<<"ERROR: found no remainder transition from state s"<<current_state.ID<<"\n";}

	return t_remainder;
}



MC state_split1(MC model)
{
	bool case1 =false;	
 statet current_state, new_state;
 transitiont t0, t1, t2, t3, t_remainder;
 std::pair<fractiont,unsigned> p1;
 std::vector<transitiont> temp;

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
			   		new_state.transitions.push_back(t1);
			   		new_state.ID = model.states.size();
			   		model.states.push_back(new_state);
			   		
			   	 //add transitions from state to new state
			   	 	t2.type = FUNCTION;
					t2.successor = new_state.ID;
					p1=t0.params[i];
					t2.params.push_back(p1);
					model.states[s_index].transitions.push_back(t2);
					//temp.push_back(t2);
					//std::cout<<temp.size();
				  }	
				model.states[s_index].transitions.erase(model.states[s_index].transitions.begin()+t_index);
				//model.states[s_index].transitions.push_back(temp); 

		 	}

	}

	}
	return model;
}


MC state_split2(MC model)
{
	bool case2=false;
	bool rem_found=false;
	statet current_state, new_state;
	transitiont t0, t1, t2, t3, t_remainder;
	std::pair<fractiont,unsigned> p1;
 	std::vector<transitiont> temp;

 	std::cout<<"MODEL SIZE:  ";
 	std::cout<<model.states.size();
 	std::cout<<"\n";
 	



 	for(unsigned s_index=0; s_index<model.states.size(); s_index++)
	 {
	 	current_state = model.states[s_index];
	 	temp = {};
	 	case2=false;
	 	t_remainder = find_remainder(current_state);

	  }


	  
	  
	/*
	  for(unsigned t_index=0; t_index<current_state.transitions.size(); t_index++)
	  {
	 	t0 = current_state.transitions[t_index];

	 		if(t0.type==FUNCTION && t0.params.size()>2)
	 		{std::cout<<"ERROR: apply state_split1 first \n";}
	 		else if (t0.type==FUNCTION && t0.params.size()>0)
	 		{
	 			case2=true;
	 			for(unsigned i=0; i<t0.params.size(); i++)
	 			{
	 				//add state
	 				t1 = {};
	 				t2 = {};
	 				new_state = {};
	 				new_state.ID = model.states.size();
	 				t1.type = FUNCTION;
	 				t1.successor = t0.successor;
	 				t1.params.push_back(t0.params[i]);
	 				t1.params[0].first.one(); //transition probability = parameter multiplied by 1
	 				t2.type = REMAINDER;
	 				t2.successor = t_remainder.successor;
	 				new_state.transitions.push_back(t1);
	 				new_state.transitions.push_back(t2);
	 				model.states.push_back(new_state);
	 				//
	 				t0.successor = new_state.ID;
	 				t0.type = CONST;
	 				t0.prob = t0.params[0].first;
	 				t0.params = {};

	 			}
	 		}
	 }*/

	//}
return model;
}





















