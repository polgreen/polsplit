#include "model.h"
#include <iostream>

/*

Is it possible to do the state splitting symbolically?? 

Can we find all states, where the probability of transitioning to a successor state
is in the form c1 X1 + c2 X2 + ... cn Xn?

*/


MC state_split(MC model)
{

 statet new_state;
 transitiont t1, t2;
 std::vector<transitiont> temp;

 for(const auto & s: model.states)
	{
	 for(const auto & t: s.transitions)
	 {
			//case 1 split
		 if(t.type==FUNCTION &&)
		 	{
		 		if(t.params.size()>1)
		 		{//add new state
		 		  for(unsigned i=0; i<t.params.size(); i++)
		 		   {	
		 			new_state.ID = model.states.size();
			   		t1.type = CONST;
			   		t1.successor = t.successor;
			   		t1.prob = 1;
			   		new_state.transitions.push_back(t1);
			   	 //add transitions from state to new state
			   	 	t2.type = FUNCTION;
					t2.successor = new_state.ID;
					t2.params.push_back(t.params.at(i));
					t2.paramindex.push_back(t.paramindex.at(i));
					temp.push_back(t2);
				  }				  
		 		}

		 		s.transition.erase(t);
		 		s.transitions.push_back(temp);
		 	}	
			//case 2 split

		}
	}
	return model;
}