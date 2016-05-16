#ifndef distributions_h
#define distributions_h

#include "fraction.h"
double Beta_probability(double lower_bound, double upper_bound, double alpha1, double alpha2);
std::vector<fractiont> parameter_distributions(MC model);
bool sample_distribution(double lower_bound, double upper_bound, double alpha1, double alpha2);

#endif