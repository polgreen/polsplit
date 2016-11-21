#ifndef statmodelchecker_h
#define statmodelchecker_h

#include <vector>
#include "markov_chain.h"
#include "pctl_parser.h"
typedef std::vector<unsigned> trace_index;
typedef std::vector<trace_index> trace_set;
typedef std::vector<statet> tracet; 
void printtrace_index(trace_index t);
trace_index tracecheck(pctlformula f, tracet t);
enum resultt {UNKNOWN, PASS, FAIL};

#endif
