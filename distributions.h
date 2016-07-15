#ifndef distributions_h
#define distributions_h

#include "fraction.h"
double Beta_probability(double lower_bound, double upper_bound, double alpha1, double alpha2);
std::vector<unsigned> parameter_distributions(MC model, std::vector<double> lower_bounds, std::vector<double> upper_bounds);

#endif