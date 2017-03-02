#ifndef distributions_h
#define distributions_h

#include <gsl/gsl_randist.h>
#include <gsl/gsl_cdf.h>
#include <gsl/gsl_rng.h>
#include "fraction.h"

double Beta_probability(double lower_bound, double upper_bound, double alpha1, double alpha2);
//td::vector<unsigned> parameter_distributions(MC model, std::vector<double> lower_bounds, std::vector<double> upper_bounds);

class random_distribution
{
public:
 random_distribution()
 {
   rng=gsl_rng_alloc(gsl_rng_taus);
 }

 ~random_distribution()
 {
   gsl_rng_free(rng);
 }

 void set_seed(unsigned long int seed)
 {
   gsl_rng_set(rng, seed);
 }

 unsigned int poisson(double mu)
 {
   return gsl_ran_poisson(rng, mu);
 }

void multinomial(size_t k, unsigned int N, const std::vector<double> &p, std::vector<unsigned> &n)
 {
  gsl_ran_multinomial(rng, k, N, p.data(), n.data());
 }

  unsigned int binomial(double p, unsigned int n)
 {
   return gsl_ran_binomial(rng, p, n);
 }

 double beta(double a, double b)
 {
 	return gsl_ran_beta(rng, a, b);
 }

private:
 gsl_rng * rng;
};

#endif