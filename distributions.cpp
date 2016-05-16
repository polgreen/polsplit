#include <vector>
#include "model.h"
#include <random>
#include <iostream>
#include <cassert>
#include "fraction.h"
#include "distributions.h"
#include <gsl/gsl_randist.h>
#include <gsl/gsl_cdf.h>
#include <gsl/gsl_rng.h>

double Beta_probability(double lower_bound, double upper_bound, double alpha1, double alpha2)
{
	double result, lower, upper;
//	std::cout<<"lower bound: "<<lower_bound<<" upper bound: "<<upper_bound<<"\n";
	lower = gsl_cdf_beta_P(lower_bound, alpha1, alpha2);
//	std::cout<<"P lower "<<lower<< " ";
	upper = gsl_cdf_beta_Q(upper_bound, alpha1, alpha2);
//	std::cout<<" P upper "<<upper<<" \n";
	result = 1 - lower - upper;
//	std::cout<<"result: "<<result<<"\n";

return result;
}

std::vector<double> parameter_distributions(MC model, std::vector<double> lower_bounds, std::vector<double> upper_bounds)
{
	std::cout<<"parameter distributions \n";
	std::vector< std::vector< std::pair < statet, unsigned> > > param_states;
	param_states = model.get_parameterised_states();
	std::vector<fractiont> result;
	std::vector<double> probabilities;
	probabilities.resize(param_states.size());
	result.resize(param_states.size());

	for(auto r: result) //initialise vector
		{r.zero();}

	for(unsigned p_index=0; p_index<param_states.size(); p_index++)
	{
		//result[p_index].denom = param_states[p_index].first.input;
		for(const auto s: param_states[p_index])
		{result[p_index].nom = result[p_index].nom + s.first.transitions[s.second].count;
		result[p_index].denom += s.first.input;}
	}

	for(unsigned i=0; i<result.size(); i++)
	{
		std::cout<<"dist: "<<result[i].nom<<" "<<result[i].denom;
		std::cout<<"\n";
		probabilities[i]=Beta_probability(lower_bounds[i], upper_bounds[i], result[i].nom, result[i].denom);
	}

return probabilities;
}


std::vector<fractiont> sample_parameter_distributions(MC model, std::vector<double> lower_bounds, std::vector<double> upper_bounds, unsigned N)
{
	std::cout<<"parameter distributions \n";
	std::vector< std::vector< std::pair < statet, unsigned> > > param_states;
	//param_states[i] = vector_of_states for parameter [i]. 
	//vector_of_states[i] = pair of {state and transition index} for a parameterised transition

	std::vector<fractiont> result;
	std::vector<double> probabilities;
	probabilities.resize(param_states.size());
	result.resize(param_states.size());
	std::vector<unsigned> sat_samples;

	for(auto r: result) //initialise vector
		{r.zero();}

	// missing data sample
	for(unsigned n=0; n<N; n++)
	{
		model.missing_data_sample();
		param_states = model.get_parameterised_states();

		for(unsigned p_index=0; p_index<param_states.size(); p_index++) //for each parameter
			{
	
				for(const auto s: param_states[p_index]) //for each state:transition pair
					{result[p_index].nom = result[p_index].nom + s.first.transitions[s.second].count;
						result[p_index].denom = s.first.input;}
			}

		for(unsigned r=0; r<result.size(); r++)
		{
			if(sample_distribution(lower_bounds[r], upper_bounds[r], result[r].nom, result[r].denom)==true)
			{result[r].nom++;}
			result[r].denom++;
		}	
 	}


	for(unsigned i=0; i<result.size(); i++)
	{
		std::cout<<"dist: "<<result[i].nom<<" "<<result[i].denom;
		std::cout<<"\n";
		probabilities[i]=Beta_probability(lower_bounds[i], upper_bounds[i], result[i].nom, result[i].denom);
	}

return result;
}




double MC::missing_data_sample()
{
	const gsl_rng_type * T;
  	gsl_rng * r;
	unsigned input;
	gsl_rng_env_setup();
  	T = gsl_rng_default;
  	r = gsl_rng_alloc (T);
  	fractiont frac;
  	double prob;
// get one sample of missing data
	for(auto &s: states)
	{
		if(s.added==false)
		{
		 for(auto & t: s.transitions)
		 {
			if(t.added==true)
			{
				//FILL PROB VALUE???????????////////////////////
				t.count = gsl_ran_binomial(r, prob, s.input);
				states[t.successor].input +=t.count;
			}
		 }
		}
	}
	for(auto &s: states)
	{
		if(s.added==true)
		{
		 for(auto & t: s.transitions)
		 {
			if(t.added==true)
			{	
				//FILL PROB VALUE???????????////////////////////
							
				t.count = gsl_ran_binomial(r, prob, s.input);
				states[t.successor].input+=t.count;
			}
		 }
		}
	}

}





bool sample_distribution(double lower_bound, double upper_bound, double alpha1, double alpha2)
{
	const gsl_rng_type * T;
  	gsl_rng * r;
	unsigned sat;
	gsl_rng_env_setup();
  	T = gsl_rng_default;
  	r = gsl_rng_alloc (T);
  	double sample;
		sample = gsl_ran_beta(r, alpha1, alpha2);
		if(sample<upper_bound && sample>lower_bound)
			{return true;}
return false;	
}
