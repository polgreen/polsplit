#include "model.h"
#include <iostream>
#include "fraction.h"

/*

Is it possible to do the state splitting symbolically?? 

Can we find all states, where the probability of transitioning to a successor state
is in the form c1 X1 + c2 X2 + ... cn Xn?

*/


MC state_split(MC model)
{
	bool case1 =false;	
 statet current_state, new_state;
 transitiont t0, t1, t2;
 std::pair<unsigned,unsigned> p1;
 std::vector<transitiont> temp;
//replace with indexes
 for(unsigned s_index=0; s_index<model.states.size(); s_index++)
	{
	current_state = model.states[s_index];	
	temp = {};
	case1=false;	
	 for(unsigned t_index=0; t_index<current_state.transitions.size(); t_index++)		
	 {
		t0 = current_state.transitions[t_index];
		//case 1 split
		 if(t0.type==FUNCTION && t0.params.size()>1)
		 	{
		 		case1=true;
		 		std::cout<<"clear temp \n";
		 		//add new state
		 		 for(unsigned i=0; i<t0.params.size(); i++)
		 		   {
		 		   	std::cout<<"param index"<<i<<" \n";
		 		   t1 = {};
		 		   t2 = {};
		 		   new_state = {};
			   		t1.type = CONST;
			   		t1.successor = t0.successor;
			   		t1.prob = 1;
			   		new_state.transitions.push_back(t1);
			   		new_state.ID = model.states.size();
			   		model.states.push_back(new_state);
			   		
			   	 //add transitions from state to new state
			   	 	t2.type = FUNCTION;
					t2.successor = new_state.ID;
					p1=t0.params[i];
					t2.params.push_back(p1);
					temp.push_back(t2);
				  }	
		 	}
		 	if(case1){model.states[s_index].transitions = temp;}

	//case 2 split
		 

	}

	}
	//model.add_IDs();
	return model;
}