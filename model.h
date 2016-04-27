#ifndef model_h
#define model_h

#include <random>
#include <vector>
enum transitiontype {CONST, FUNCTION};

struct transitiont {
	transitiontype type; 
	unsigned successor; 
	unsigned prob; 
	std::vector <std::pair < unsigned, unsigned> > params;
//	std::vector<unsigned> params; 
//	std::vector<unsigned> paramindex;
	};
struct statet {
	unsigned ID; 
	std::vector<transitiont> transitions; 
	bool init; 
	unsigned label;
	};
struct MC {
	std::vector<statet> states; 
	std::vector<unsigned> params; 
	void outputMC(std::ostream &out);
	statet get_init_state();
	void add_IDs();
	};
typedef std::vector<statet> tracet; 

MC get_ZeroConfMC(unsigned probes, unsigned lossWT, unsigned notlossWT, 
	unsigned useWT, unsigned notuseWT);
void printstate(statet s);
tracet gettrace(std::default_random_engine &generator, MC model, unsigned length);
void printtrace(tracet trace);

MC get_parameterisedMC (unsigned probes, unsigned lossWT, 
	unsigned notlossWT, unsigned useWT, unsigned notuseWT);
MC state_split(MC model);
#endif