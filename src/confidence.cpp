#include <vector>
#include <random>
#include <time.h>
#include <random>
#include <iostream>
#include <cassert>
#include "fraction.h"
#include "distributions.h"
#include "MC.h"

void MC::reset_confidence()
{
  if (verbose > 1)
    std::cout << "reset confidence\n";
  overall_confidence.zero();
}

void MC::sample_params_update_conf(random_distribution &rd)
{
  if (verbose > 3)
    std::cout << "\nsample params and update confidence \n";

  std::vector<double> sample;
  for (unsigned i = 1; i < modelparams.size(); i++)
  {
    sample.push_back(
        rd.beta(parametercounts[i] + prior_a1[i], inv_parametercounts[i] + prior_a2[1]));
  }
  if (is_in_range(sample, true))
  {
    overall_confidence.nom++;
  }
  overall_confidence.denom++;
  if (verbose > 1)
    std::cout << "confidence = " << overall_confidence.nom << "/"
        << overall_confidence.denom << "\n";
}

fractiont MC::confidencecalc(const bool reset, const unsigned num_samples)
{
  if (verbose > 1)
    std::cout << "confidence calc, number samples " << num_samples << "\n";
  unsigned seed = 0;
  random_distribution rd;
  if (verbose > 1)
    std::cout << "Using seed of " << seed << "\n";
  rd.set_seed(seed); //we use the same seed so that we can reproduce experiments
  std::vector<fractiont> result(modelparams.size(), 0);
  std::vector<std::pair<statet, unsigned> > param_states;
  if (reset)
    reset_confidence();

  param_states = get_parameterised_states();

  for (unsigned i = 0; i < num_samples; i++)
  {
    if (need_state_splitting)
    {
      if (verbose > 1)
        std::cout << "State splitting \n";
      get_random_model_params(rd);
      sample_D_star(param_states, rd);
    }
    sample_params_update_conf(rd);

  }
  if (verbose > 0)
    std::cout << "MC confidence " << overall_confidence.nom << "/"
        << overall_confidence.denom << "\n";
  ;
  return overall_confidence;
}

