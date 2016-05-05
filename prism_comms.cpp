#include <vector>
#include "model.h"
#include <random>
#include <iostream>
#include <cassert>
#include "fraction.h"
#include <sstream>
#include <fstream>
#include <cstdlib>
#include "pctl_parser.h"



std::string ssystem (const char *command) 
{
    char tmpname [L_tmpnam];
    std::tmpnam ( tmpname );
    std::string scommand = command;
    std::string cmd = scommand + " >> " + tmpname;
    std::system(cmd.c_str());
    std::ifstream file(tmpname, std::ios::in );
    std::string result;
        if (file) {
      while (!file.eof()) result.push_back(file.get());
          file.close();
    }
    remove(tmpname);
    return result;
}


void MC::PRISMsynthesis(pctlformula property)
{
	if(property.t.kind==INIT)
		{std::cout<<"Property is undefined \n"; throw std::exception();}
	std::ofstream prismfile ("prismfile.pm");
	std::ofstream propertyfile ("propertyfile.props");
	if(!prismfile || !propertyfile){throw std::exception();}
	outputPRISM(prismfile);
	outputproperty(property, propertyfile);
	prismfile.close();
	propertyfile.close();
	std::string result;
	char letter='a';
	std::string command ("prism prismfile.pm propertyfile.props -param ");
	for(unsigned p_index=1; p_index<modelparams.size(); p_index++)
	{
		command+=static_cast<char>(letter+p_index -1);
		command+="=0:1,";
	}
	system(command.c_str());	
}


void MC::outputPRISM(std::ostream &out)
{
	char letter = 'a';
	out <<"dtmc \n\n";

	for(unsigned p_index = 1; p_index<modelparams.size(); p_index++)
		{out<<"const double ";
			out<<static_cast<char>(letter + p_index-1)<<";\n";}
	out<<"\n\nmodule test \n";
	out<<"\n // local state \n s: [0.."
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
						if(p.second>0){out<<static_cast<char>(letter + p.second -1);}
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
			                              if(p2.second>0){out<<static_cast<char>(letter + p2.second -1);}
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
		out<<"\nendmodule \n \n";
		out<< "label \"complete\"=(s=5);\n";

}



