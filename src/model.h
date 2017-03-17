#ifndef model_h
#define model_h

#include <random>
#include <vector>
#include <fstream>
#include "fraction.h"
#include "distributions.h"

enum transitiontype {
    CONST, FUNCTION, REMAINDER, NEWFUNCTION, NEWCONST, NEWREMAINDER
};

enum transadded {
    T1, T2, T3, T0, T4
};

enum statetype {
    S1, S2, S3, S0
};

struct transitiont {

    transitiont() {
        count = 0;

    }
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

    statet() {
        input = 0;
    }
    unsigned ID;
    std::vector<transitiont> transitions;
    bool init;
    unsigned label;
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
    std::vector<unsigned> beta_prior_param1;
    std::vector<unsigned> beta_prior_param2;
    std::vector<double> param_lower_bounds;
    std::vector<double> param_upper_bounds;
    std::vector<fractiont> param_confidence;
    fractiont overall_confidence;
    unsigned success = 2;
    random_distribution rd;
    unsigned model_num;

    std::vector<std::vector<std::pair <double, double > > >parameter_bounds;
    std::vector<std::string> parameter_results;
    unsigned verbose;
    unsigned trace_length;
    unsigned number_of_traces;
    long int_samples;
    bool need_state_splitting = true;

    //debugging
    void outputMC(std::ostream &out);


    //building and checking models
    void check();

    statet get_init_state();
    void add_IDs();
    fractiont weighting(transitiont t, statet s);
    fractiont remainderWeight(statet s);
    void prepModel();

    //parameter synthesis
    void callPrism();
    void virtual outputPRISM(std::ostream &out);
    void prism_find(std::string&);

    //get trace data
    std::vector< std::pair < statet, unsigned> > get_parameterised_states();
    void get_data();
    tracet virtual gettrace();
    void virtual get_trace_counts(tracet&);
    void displayConfidence();


    //void sample_transition_counts(random_distribution &)
    void confidencecalc();
    void reset_confidence();
    void get_random_model_params();
    void sample_D_star(std::vector< std::pair < statet, unsigned> > &);
    void sample_params_update_conf();
    void updatePriors();

    //check if params are in feasible bounds
    bool is_in_range(std::vector<double>&);
    bool result_bound_satisfied(unsigned, std::vector<double>&);
    unsigned virtual getStateIndex(unsigned);
    double Generate(const double, const double);
    void initRndDistribution();
};

std::ofstream getPRISMFile();
std::string prepCmd();
MC get_simpleMC();
std::string doubleToString(double);


//debugging
void printstate(statet s);
void printtrace(tracet trace);


#endif
