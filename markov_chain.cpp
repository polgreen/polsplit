 /*****************************\
          (\.---./)    
           /.-.-.\
          /| 0_0 |\
         |_`-(v)-'_|
         \`-._._.-'/      .-.
   -~-(((.`-\_|_/-'.)))-~' <_
          `.     .'
            `._.'
          BaeVer
 Bayesian Verification for DTMCs     
   -----~--~---~~~----~-`.-;~
   elizabeth.polgreen@cs.ox.ac.uk
\********************************/



#include "markov_chain.h"
#include <iostream>



statet get_init_state()
{
 statet init;
 init.hasbus = 1;
 init.clients=10;
return init;
}

std::vector<std::pair<statet,unsigned> > get_successors(statet current)
{
 std::vector<std::pair<statet,unsigned> > result;
 for (unsigned i=0; i<current.clients; i++)
 {
 	statet successor;
 	successor.clients=current.clients;
 	successor.hasbus = i;
 	//add labels////////////////
 	if(i==3 || i==6)
 		{successor.label.push_back(1);}
 	else if(i==4 || i==8)
 		{successor.label.push_back(1);}
 	else{successor.label.push_back(0);}
 	//////////////////////////////
 	std::pair<statet,unsigned> p;
 	p.first = successor;
 	p.second = 1;
 	result.push_back(p);
 }
return result;
}

void printstate(statet s) {
	std::cout<<s.hasbus<<":";
	for(const auto &l: s.label)
	{
		std::cout<<l;
	}
	std::cout<<" ";
}

satset get_all_states()
{
	satset allstates;
	statet s;
	for(unsigned i=0; i<10; i++)
	{
		s.hasbus=i;
		s.clients=10;
		allstates.push_back(s);
	}
	return allstates;
}

satset get_successor_states(statet current)
{
 	satset result;
	 std::vector<std::pair<statet,unsigned> > successors;
	 successors = get_successors(current);
	for(const auto & s1: successors)
	{
		if(s1.second>0)
			{result.push_back(s1.first);}
	} 
return result;
}



