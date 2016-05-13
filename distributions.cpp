#include <vector>
#include "model.h"
#include <random>
#include <iostream>
#include <cassert>
#include "fraction.h"
#include "distributions.h"
#include <gsl/gsl_randist.h>
#include <gsl/gsl_cdf.h>

double Beta_probability(double lower_bound, double upper_bound, double alpha1, double alpha2)
{
	double result, lower, upper;
	lower = gsl_cdf_beta_P(lower_bound, alpha1, alpha2);
	upper = gsl_cdf_beta_Q(upper_bound, alpha1, alpha2);
	result = 1 - lower - upper;
return result;
}