#include "statmodelchecker.h"
#include "markov_chain.h"
#include "pctl_parser.h"
#include "propcheck.h"
#include <algorithm> 





/*
void printtrace_index(trace_index trace)
{
	for(const auto &s: trace) 
    {
        std::cout<<s<<" ";
    }
    std::cout<<"\n";
}

trace_set tracecheckIDENTIFIER (tracet traces, pctlformula f)
{
	trace_index result;
	trace_set result_set;

		for(unsigned i=0; i<trace.size(); i++)
		{
			if(trace.at(i).label == f.t.label)
			{
			 result.push_back(i);
			}
			else
			{
			std::cout<<"IDENTIFIER: ";
			printtrace_index(result);
			result_set.push_back(result);
			result.clear();
			}
		} 
	return result_set; 
}

trace_set tracecheckAND(trace_set set1, trace_set set2)
{
	trace_index result;
	trace_set result_set;

	for(const auto & s1: set1)
	{
		for(const auto & s : s1 )
		{
			for(const auto &s2 : set2)
			{
				if(std::find(s2.begin(), s2.end(), s)!=s2.end())
					{result.push_back(s);}
				else
					{std::cout<< "AND: ";
					printtrace_index(result);
					result_set.push_back(result);
					result.clear();	}

			}
		}
	}		
	return result_set;
}



trace_set tracecheckOR(trace_set set1, trace_set set2)
{
 for(const auto & s1 : set1)
 {
   if(std::find(set2.begin(), set2.end(), s1)==set2.end())
     set2.push_back(s1);
 } 
	std::cout<<"OR: ";
	printtrace_index(set2);
 return set2;
}

trace_set tracecheckNOT(trace_set set1, tracet trace)
{
	bool found = false;
	trace_index result;
	for(unsigned i=0; i<trace.size(); i++)
	{
		for( const auto & s1: set1)
		{
			if (s1==i) 
				{found=true;}
		}
		if(found==false) 
			{result.push_back(i);}
	}
	std::cout<<"NOT: ";
	printtrace_index(result);
	return result;
}


trace_set tracecheckUNTIL(trace_set set1, ttrace_set set2)
{
	trace_index result;
	if(!set2.empty())
	{

	}
}

*/
trace_set tracecheck(pctlformula f, trace_set t)
{

	
	switch (f.t.kind)
	{
//	 case IDENTIFIER: return tracecheckIDENTIFIER(t, f); break;	
//     case UNTIL: return tracecheckUNTIL(tracecheck(f.children[0]), tracecheck(f.children[1]) ); break;
//	 case AND: return tracecheckAND(tracecheck(f.children[0],t), tracecheck(f.children[1],t)); break;
// 	 case OR:  return tracecheckOR(tracecheck(f.children[0],t), tracecheck(f.children[1],t)); break;
// 	 case PROB: ;break;
//	 case NOT: return tracecheckNOT(tracecheck(f.children[0],t),t); break;
 	 case GT: ;break;
 	 case LT: ;break;
 	 case EQ: ;break;
 	 case LP: ; break;
 	 case RP: ; break;
 	 case SP: ; break;	
 	 case FI: ; break;
 	 case GL: ; break;
 	 case X: ; break;
 	 case IMPLIES: ; break;
 	 case GE: ; break;
 	 case LE: ; break;
 	 default:;

	}

}