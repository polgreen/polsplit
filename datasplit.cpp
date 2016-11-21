#include "model.h"
#include <iostream>
#include "fraction.h"







MC data_split1(MC model)
{
	bool case1 =false;	
	bool remainderfound=false;
 statet current_state, new_state;
 transitiont t0, t1, t2, t3, t_remainder;
 std::pair<fractiont,unsigned> p1;
 std::vector<transitiont> temp;
 unsigned input_count;
 if(model.states.size()==0)
 {	std::cout<<"error in state_split, no states found \n";
	throw std::exception();
 }

 for(unsigned s_index=0; s_index<model.states.size(); s_index++)
	{
	current_state = model.states[s_index];	
	temp = {};
	case1=false;	

	 for(unsigned t_index=0; t_index<current_state.transitions.size(); t_index++)		
	 {
		t0 = current_state.transitions[t_index];

		 if(t0.type==FUNCTION && t0.params.size()>1)
		 	{
		 		case1=true;
		 		if(remainderfound==false){
		 			t_remainder = find_remainder(current_state);
		 			remainderfound=true;}
		 			input_count = t0.count;

		 		 for(unsigned i=0; i<t0.params.size(); i++)
		 		   {
		 		   t1 = {};
		 		   t2 = {};
		 		   	new_state = {};
			   		t1.successor = t0.successor;
			   		t1.prob.one();
			   		new_state.newtype = S1;
			   		new_state.transitions.push_back(t1);
			   		new_state.ID = model.states.size();


			   		model.states.push_back(new_state);
			   		
			   	 //add transitions from state to new state
			   	 	t2.type = NEWFUNCTION;
			   	 	t2.newtype = T3;
					t2.successor = new_state.ID;
					t2.countknown=false;
					p1=t0.params[i];
					t2.params.push_back(p1);
					model.states[s_index].transitions.push_back(t2);
					model.states[s_index].newtype=S3;
					//temp.push_back(t2);
					//std::cout<<temp.size();--ik
				  }	
				model.states[s_index].transitions.erase(model.states[s_index].transitions.begin()+t_index);
				model.states[s_index].inputknown=true;
				model.states[s_index].outputknown=false;

		 	}
		 	if(t0.type==FUNCTION && t0.params.size()==0)
		 		{std::cout<<"Error in state_split1, state "<<current_state.ID<<" has FUNCTION transition with no parameters\n";

		 		throw std::exception();}
	}

	}
	return model;
}


MC data_split2(MC model)
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

 	//std::cout<<"MODEL SIZE:  ";
 	//std::cout<<model.states.size();
 	//std::cout<<"\n";
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
	 		if(t0.type==NEWFUNCTION || t0.type==FUNCTION)
	 		{
	 			if(t0.params.size()==1)
	 			{
	 		//		std::cout<<"FUNCTION: params: "<<t0.params.size()<<"\n";
	 				t1={};
	 				t2={};
	 				new_state={};
	 				new_state.ID = model.states.size() + temp_states.size();
	 				new_state.inputknown=false;
	 				new_state.outputknown=false;
	 				new_state.newtype=S2;
	 				t1.type = NEWFUNCTION;
	 				t1.newtype = T2;
	 				t1.successor = t0.successor;
	 				t1.countknown=false;
	 				t1.params.push_back(t0.params[0]);
	 				t1.params[0].first.one(); //transition probability = parameter multiplied by 1
	 				if(t0.countknown){t1.count = t0.count;t1.countknown=true;}
	 				else{t1.countknown=false;} //transition count is same as original deleted transition
	 				t2.type = NEWREMAINDER;
	 				t2.newtype=T4;
	 				t2.countknown=false;
	 				t2.successor = t_remainder.successor;
	 				new_state.transitions.push_back(t1);
	 				new_state.transitions.push_back(t2);
	 				new_state.newtype=S3;
	 				temp_states.push_back(new_state);
	 				t3.successor = new_state.ID;
	 				t3.type = NEWCONST;
	 				t3.newtype=T2;
	 				t3.countknown=false;
	 				t3.prob = t0.params[0].first;
	 				model.states[s_index].transitions[t_index] = t3;
	 				model.states[s_index].outputknown=false;
	 			}
	 			else{std::cout<<"error in state_split2: please apply state split 1 first \n";
	 				throw std::exception();}
	 		}
	 	}
	 }
	 for(const auto & s: temp_states)
	 {
	 	//s.inputknown=false; s.outputknown=false;
	 	model.states.push_back(s);
	 }

return model;
}
























