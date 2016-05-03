#include <vector>
#include "model.h"
#include <random>
#include <iostream>
#include <cassert>
#include "fraction.h"




void MC::outputPRISM(std::ostream &out)
{
	
	
	char letter = 'a';
	out <<"dtmc \n\n";
	out << "const double ";
	for(unsigned p_index = 1; p_index<modelparams.size(); p_index++)
	{out<<static_cast<char>(letter + p_index)<<" ";}
	out<<";\n\nmodule test \n";
	out<<"\n // local state \n s: [0..."
		<<states.size()-1<<"] init "<<get_init_state().ID;
	out<<";\n \n";

	for(auto & s: states)
	{
	 out<<"[] s="<<s.ID<<" -> ";
	 	bool first_t=true;
		for(auto & t: s.transitions)
		{
			if(first_t){first_t=false;}
			else{out<<" + ";}
			bool first_p=true;
			switch(t.type)
			{
				case CONST: t.prob.output(out);out<<" : (s'="<<t.successor<<") "; break;
				case FUNCTION:
				  for(auto & p: t.params)
					{
						if(first_p){first_p=false;}
						else{out<< " + ";}
							p.first.output(out);
							out<<"*";
						if(p.second>0){out<<static_cast<char>(letter + p.second);}
					} out<<" : (s'="<<t.successor<<")"; break;
				case REMAINDER: out<<"(1";
						for(auto & t2: s.transitions)
							switch(t2.type)
						     {
							  case CONST: out<<"-"; t2.prob.output(out); break;
							  case FUNCTION: 
							  		for(auto & p2: t2.params)
										{
											out<<"-";
										  p2.first.output(out);
										  out<<"*";
			                              if(p2.second>0){out<<static_cast<char>(letter + p2.second);}
						    			 }
									break;	
							  case REMAINDER: break;	
				            default:;
				           }
				           out<<"):(s'="<<t.successor<<")";
				 default:;          
			}

		}out<<";\n";
	}
		out<<"\nendmodule \n";
}


//MC getPRISM(std::ifstream file)
//{



//}/