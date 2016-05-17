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

	std::cout<<"lower, upper, alpha1, alpha2: "<<lower_bound<<" "<<upper_bound<<" "<<alpha1<<" "<<alpha2<<"\n";
		if(alpha1==0 ||alpha2==0)
		{std::cout<<"Error, alpha value = 0 \n";
		throw std::exception();}
	lower = gsl_cdf_beta_P(lower_bound, alpha1, alpha2);
	upper = gsl_cdf_beta_Q(upper_bound, alpha1, alpha2);
	result = 1 - lower - upper;

return result;
}

std::vector<double> parameter_distributions(MC model, std::vector<double> lower_bounds, std::vector<double> upper_bounds)
{
	std::cout<<"parameter distributions \n";
	std::vector< std::vector< std::pair < statet, unsigned> > > param_states;
	param_states = model.get_parameterised_states();
	std::vector<fractiont> result;
	std::vector<double> probabilities;
	unsigned placeholder;
	probabilities.resize(param_states.size());
	result.resize(param_states.size());
	if(lower_bounds.size()!=model.modelparams.size()||lower_bounds.size()!=upper_bounds.size()
		||param_states.size()!=model.modelparams.size())
	{
		std::cout<<"Error: lower_bounds must match upper_bounds and params in size \n";
		throw std::exception();
	}

	for(unsigned r=0; r<result.size(); r++)
	{
		result[r].one();
	}


	for(unsigned p_index=0; p_index<param_states.size(); p_index++)
	{
		//result[p_index].denom = param_states[p_index].first.input;
		for(const auto s: param_states[p_index])
		{result[p_index].nom = result[p_index].nom + s.first.transitions[s.second].count;
		result[p_index].denom += s.first.input - s.first.transitions[s.second].count;}
	}

	for(unsigned i=0; i<result.size(); i++)
	{
		std::cout<<"dist: "<<result[i].nom<<" "<<result[i].denom;
		std::cout<<"\n";
		probabilities[i]=Beta_probability(lower_bounds[i], upper_bounds[i], result[i].nom, result[i].denom);
		//std::cout<<probabilities[i];
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

  	//fill parameter values from uniform prior
  	for(unsigned p_index=0; p_index<modelparams.size(); p_index++)
  	{modelparams[p_index] = gsl_ran_beta(r,1,1);}

	for(auto &s: states)
	{
		if(s.added==false)
		{
		 for(auto & t: s.transitions)
		 {
			if(t.added==true)
			{
				frac = weighting(t,s);
				prob = frac.nom/frac.denom;
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
				frac = weighting(t,s);
				prob = frac.nom/frac.denom;				
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
