#ifndef model_h
#define model_h

#include <random>
#include <vector>
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
        countknown = true;
        newtype = T0;
    }
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

    statet() {
        input = 0;
        inputknown = true;
        outputknown = true;
        newtype = S0;
    }
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
    unsigned success = 2;

    std::vector<std::vector<std::pair <double, double > > >parameter_bounds;
    std::vector<std::string> parameter_results;
    unsigned verbose;
    unsigned trace_length;
    unsigned number_of_traces;


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
    std::vector< std::pair < statet, unsigned> > get_parameterised_states();
    void get_data(unsigned, random_distribution &);
    tracet gettrace(unsigned, random_distribution &);
    void get_trace_counts(tracet&);



    //void sample_transition_counts(random_distribution &)
    void confidencecalc(unsigned num_samples);
    void reset_confidence();
    void get_random_model_params(random_distribution &);
    void sample_D_star(std::vector< std::pair < statet, unsigned> > &, random_distribution &);
    void sample_params_update_conf(random_distribution &);

    //check if params are in feasible bounds
    bool is_in_range(std::vector<double>&);
    bool result_bound_satisfied(unsigned, std::vector<double>&);


};


MC get_simpleMC();

//debugging
void printstate(statet s);
void printtrace(tracet trace);


#endif
