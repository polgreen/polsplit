#ifndef SRC_MC_H
#define SRC_MC_H

#include <random>
#include <vector>
#include "fraction.h"
#include "distributions.h"
enum transitiontype
{
  CONST, FUNCTION, REMAINDER
};

struct transitiont
{
    transitiont()
    {
      count = 0;
    }
    transitiontype type;
    unsigned successor;
    fractiont prob;
    std::vector<std::pair<fractiont, unsigned> > params;
    unsigned count;
};

struct statet
{
    statet()
    {
      input = 0;
      init = false;
    }
    unsigned ID;
    std::vector<transitiont> transitions;
    bool init;
    unsigned input;
    unsigned sum_outputs();
};

typedef std::vector<statet> tracet;
typedef std::vector<fractiont> countt;

struct MC
{
    MC()
    {
      num_int_samples=1000;
      success=2;
    }
    std::vector<statet> states;
    //note that slot 0 in modelparams is 1, and used
    //to represent known constants in param functions
    std::vector<fractiont> modelparams;
    std::vector<unsigned> prior_a1;
    std::vector<unsigned> prior_a2;
    std::vector<unsigned> parametercounts;
    std::vector<unsigned> inv_parametercounts;
    std::vector<fractiont> param_confidence;
    fractiont overall_confidence;
    std::vector<double> param_lower_bounds;
    std::vector<double> param_upper_bounds;
    unsigned success; //success state in property.

    std::vector<std::vector<std::pair<double, double> > > parameter_bounds;
    std::vector<std::string> parameter_results;

    unsigned verbose;
    unsigned trace_length;
    unsigned number_of_traces;
    unsigned num_int_samples;
    bool need_state_splitting = true;

    //debugging
    void outputMC(std::ostream &out);
    void check();
    void initialise_all_counts();

    //models
    statet get_init_state();
    void add_IDs();
    fractiont weighting(const transitiont&, const statet&);
    fractiont remainderWeight(const statet &);

    //parameter synthesis
    void callPrism();
    void outputPRISM(std::ostream &);
    void prism_find(const std::string&);
    bool check_for_parameters();
//get trace data
    std::vector<std::pair<statet, unsigned> > get_parameterised_states();
    void get_data(const unsigned, random_distribution &);
    tracet gettrace(const unsigned, random_distribution &);
    void get_trace_counts(const tracet&);

    //void sample_transition_counts(random_distribution &)
    fractiont confidencecalc(const bool, const unsigned, random_distribution &);
    void reset_confidence();
    void get_random_model_params(random_distribution &);
    void sample_D_star(const std::vector<std::pair<statet, unsigned> > &,
        random_distribution &);
    void sample_params_update_conf(random_distribution &);

//check if params are in feasible bounds
    bool is_in_range(const std::vector<double>&, const bool);
    bool result_bound_satisfied(const unsigned, const std::vector<double>&);

    std::vector<std::vector<fractiont> > transitioncountvector();

    fractiont operator()(random_distribution &);
    fractiont operator()(random_distribution &, bool);

};

MC get_MC();

//debugging
void printstate(statet &s);
void printtrace(tracet &trace);

#endif //SRC_MC_H

