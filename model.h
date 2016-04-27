#ifndef model_h
#define model_h

#include <random>
#include <vector>
enum transitiontype {CONST, FUNCTION};

struct transitiont {transitiontype type; unsigned successor; unsigned prob; std::vector<unsigned> params; std::vector<unsigned> paramindex;};
struct statet {unsigned ID; std::vector<transitiont> transitions; bool init; unsigned label;};
struct MC {std::vector<statet> states; std::vector<unsigned> params; };
typedef std::vector<statet> tracet; 

MC get_ZeroConfMC(unsigned probes, unsigned lossWT, unsigned notlossWT, unsigned useWT, unsigned notuseWT);
void printstate(statet s);
tracet gettrace(std::default_random_engine &generator, MC model, unsigned length);
void printtrace(tracet trace);
void outputMC (MC model);

#endif