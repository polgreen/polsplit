#ifndef model_h
#define model_h

#include <random>
#include <vector>
#include "fraction.h"
enum transitiontype {CONST, FUNCTION, REMAINDER};


struct transitiont {
	transitiontype type; 
	unsigned successor; 
	fractiont prob; 
	std::vector <std::pair < fractiont, unsigned> > params;
	//first = parameter multiplier
	//second = parameter index
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
	std::vector<fractiont> modelparams; 
	void outputMC(std::ostream &out);
	statet get_init_state();
	void add_IDs();
	void outputPRISM(std::ostream &out);
	fractiont weighting(transitiont t, statet s);
	fractiont remainderWeight(statet s);
	void check();
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
fractiont remainderWeight(statet s);
#endif