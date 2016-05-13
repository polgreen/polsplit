#ifndef model_h
#define model_h

#include <random>
#include <vector>
#include "fraction.h"
#include "pctl_parser.h"
enum transitiontype {CONST, FUNCTION, REMAINDER};




struct transitiont {
	transitiont(){count=0;}
	transitiontype type; 
	unsigned successor; 
	fractiont prob; 
	std::vector <std::pair < fractiont, unsigned> > params;
	unsigned count;
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

	typedef std::vector<statet> tracet; 
	typedef std::vector<fractiont> countt;

struct MC {
	std::vector<statet> states; 
	std::vector<fractiont> modelparams; 
	void outputMC(std::ostream &out);
	statet get_init_state();
	void add_IDs();
	void outputPRISM(std::ostream &out);
	fractiont weighting(transitiont t, statet s);
	fractiont remainderWeight(statet s);
	std::vector< std::vector< std::pair < statet, unsigned> > >  get_parameterised_states();
	void get_trace_counts(tracet trace);
	void PRISMsynthesis(pctlformula property);
	void check();
	};

	
std::vector<fractiont> parameter_distributions(MC model);


MC get_simpleMC();
MC get_ZeroConfMC(unsigned probes, unsigned lossWT, unsigned notlossWT, 
	unsigned useWT, unsigned notuseWT);
void printstate(statet s);
tracet gettrace( MC model, unsigned length);
void printtrace(tracet trace);

MC get_parameterisedMC (unsigned probes, unsigned lossWT, 
	unsigned notlossWT, unsigned useWT, unsigned notuseWT);
MC state_split1(MC model);
MC state_split2(MC model);
fractiont remainderWeight(statet s);
#endif