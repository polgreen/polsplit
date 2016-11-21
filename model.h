#ifndef model_h
#define model_h

#include <random>
#include <vector>
#include "fraction.h"
#include "pctl_parser.h"
#include "distributions.h"
enum transitiontype {CONST, FUNCTION, REMAINDER, NEWFUNCTION, NEWCONST, NEWREMAINDER };
enum transadded {T1, T2, T3, T0, T4};
enum statetype {S1, S2, S3, S0};




struct transitiont {
	transitiont(){count=0; countknown=true; newtype=T0;}
	transitiontype type; 
	unsigned successor; 
	fractiont prob; 
	std::vector <std::pair < fractiont, unsigned> > params;
	unsigned count;
	bool countknown;
	transadded newtype;
	//first = parameter multiplier
	//second = parameter index
//	std::vector<unsigned> params; 
//	std::vector<unsigned> paramindex;
	};
struct statet {
	statet(){input=0;inputknown=true; outputknown=true; newtype=S0;}
	unsigned ID; 
	std::vector<transitiont> transitions; 
	bool init; 
	unsigned label;
	bool outputknown;
	bool inputknown;
	unsigned input;
	statetype newtype;
	unsigned sum_outputs();
	};

	typedef std::vector<statet> tracet; 
	typedef std::vector<fractiont> countt;

struct MC {
	std::vector<statet> states; 
	std::vector<fractiont> modelparams; 
	std::vector<unsigned> parametercounts;
	std::vector<unsigned> inv_parametercounts;
	std::vector<fractiont> confidence;
	fractiont overall_confidence; 

	std::vector<std::vector<std::string > >parameter_bounds;
	std::vector<std::string> parameter_results;


	void outputMC(std::ostream &out);
	statet get_init_state();
	void add_IDs();
	void outputPRISM(std::ostream &out);
	fractiont weighting(transitiont t, statet s);
	fractiont remainderWeight(statet s);
	std::vector< std::pair < statet, unsigned> >  get_parameterised_states();
	void get_trace_counts(tracet trace);
	void PRISMsynthesis(pctlformula property);
	void check();
	double missing_data_sample();
	void sample_transition_counts(random_distribution &);
	void confidencecalc(unsigned num_samples, std::vector<double> lower_bounds, std::vector<double> upper_bounds);
	std::vector<double> parameter_distributions(std::vector<double> lower_bounds, std::vector<double> upper_bounds);
	void get_random_model_params(random_distribution &);
	std::vector<unsigned> sample_transition(unsigned, transitiont, random_distribution &);
	void sample_params_update_conf(random_distribution &,
    std::vector<double> &, std::vector<double> & );
    void sample_D_star(std::vector< std::pair < statet, unsigned> > &, random_distribution &);
    void reset_confidence();
    void prism_find(std::string);
    bool is_in_range(std::vector<double>&);
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