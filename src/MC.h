#ifdef MarkovChain
#ifndef SRC_MC_H
#define SRC_MC_H

#include <random>
#include <vector>
#include "fraction.h"
#include "distributions.h"
enum transitiontype {CONST, FUNCTION, REMAINDER};




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
	unsigned success = 2; //success state in property.

	std::vector<std::vector<std::pair <double, double > > >parameter_bounds;
	std::vector<std::string> parameter_results;
	unsigned verbose;
	unsigned trace_length;
	unsigned number_of_traces;
	unsigned numbersamples = 1000;


	//debugging
	void outputMC(std::ostream &out);


	//building and checking models
  void check();

	statet get_init_state();
	void add_IDs();
  fractiont weighting(transitiont t, statet s);
  fractiont remainderWeight(statet s);

	//parameter synthesis
	void callPrism();
	void outputPRISM(std::ostream &out);
	void prism_find(std::string&);

//get trace data
	std::vector< std::pair < statet, unsigned> >  get_parameterised_states();
	void get_data(unsigned);
  tracet gettrace(unsigned );
	void get_trace_counts(tracet&);



	//void sample_transition_counts(random_distribution &)
  fractiont confidencecalc(unsigned num_samples);
  void reset_confidence();
	void get_random_model_params(random_distribution &);
	void sample_D_star(std::vector< std::pair < statet, unsigned> > &, random_distribution &);
  void sample_params_update_conf(random_distribution &);

//check if params are in feasible bounds
  bool is_in_range(std::vector<double>&);
  bool result_bound_satisfied(unsigned, std::vector<double>& );

  fractiont operator()();

	};


MC get_MC();

//debugging
void printstate(statet s);
void printtrace(tracet trace);


#endif //SRC_MC_H
#endif
