#ifndef model_h
#define model_h

#include <random>
#include <vector>
struct statet {unsigned ID; std::vector<unsigned> successors; std::vector<unsigned> prob; bool init; unsigned label;};
struct MC {std::vector<statet> states; unsigned num; };
typedef std::vector<statet> tracet; 

MC get_ZeroConfMC(unsigned probes, unsigned lossWT, unsigned notlossWT, unsigned useWT, unsigned notuseWT);
void printstate(statet s);
tracet gettrace(std::default_random_engine &generator, MC model, unsigned length);
void printtrace(tracet trace);

#endif