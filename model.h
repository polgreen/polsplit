#ifndef model_h
#define model_h

#include <random>
#include <vector>
#include "fraction.h"
#include "pctl_parser.h"
enum transitiontype {CONST, FUNCTION, REMAINDER, NEWFUNCTION, NEWCONST, NEWREMAINDER };




struct transitiont {
	transitiont(){count=0; countknown=true;}
	transitiontype type; 
	unsigned successor; 
	fractiont prob; 
	std::vector <std::pair < fractiont, unsigned> > params;
	unsigned count;
	bool countknown;
	//first = parameter multiplier
	//second = parameter index
//	std::vector<unsigned> params; 
//	std::vector<unsigned> paramindex;
	};
struct statet {
	statet(){input=0;inputknown=true; outputknown=true;}
	unsigned ID; 
	std::vector<transitiont> transitions; 
	bool init; 
	unsigned label;
	bool outputknown;
	bool inputknown;
	unsigned input;
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
	double missing_data_sample();
	std::vector<double> sample_parameter_distributions(std::vector<double> lower_bounds, std::vector<double> upper_bounds, unsigned N);
	std::vector<double> parameter_distributions(std::vector<double> lower_bounds, std::vector<double> upper_bounds);
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