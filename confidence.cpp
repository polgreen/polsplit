#include <vector>
#include <random>
#include <time.h>
#include "model.h"
#include <random>
#include <iostream>
#include <cassert>
#include "fraction.h"
#include "distributions.h"

void MC::reset_confidence()
{
  confidence.resize(modelparams.size());
  for(auto c: confidence)
  { 
    c.nom = 0;
    c.denom = 0;
  }
}


void MC::sample_params_update_conf(random_distribution &rd)
{
  std::cout<<"sample params and update confidence \n";
  std::vector<double> sample;
  for(unsigned i=0; i<modelparams.size(); i++)
    {sample.push_back(rd.beta(parametercounts[i],inv_parametercounts[i]));}
  if(is_in_range(sample))
    {overall_confidence.nom++;}
  overall_confidence.denom++;
}


fractiont computeconfidence(std::vector<fractiont> conf_vector)
{
  fractiont result = 1;
  for(const auto c: conf_vector)
    {result = result * c;}

 return result; 
}


void MC::confidencecalc(
  unsigned num_samples)
{
  std::cout<<"confidence calc \n";

  random_distribution rd;
  rd.set_seed(0);  //we use the same seed so that we can reproduce experiments
  std::vector<fractiont> result(modelparams.size(), 0);
  std::vector< std::pair < statet, unsigned> > param_states;
  reset_confidence();

  param_states = get_parameterised_states();

  for(unsigned i=0; i<num_samples; i++)
  {
    get_random_model_params(rd);
    sample_D_star(param_states, rd);
    sample_params_update_conf(rd);

  }
  std::cout<<"overall confidence "<<overall_confidence;

}

