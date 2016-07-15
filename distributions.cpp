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
#include <sys/time.h>


unsigned long int random_seed()
{
 struct timeval tv;
 gettimeofday(&tv,0);
 return (tv.tv_sec + tv.tv_usec);
}

bool sample_distribution(double lower_bound, double upper_bound, double alpha1, double alpha2)
{
	const gsl_rng_type * T;
  	gsl_rng * r;
	gsl_rng_env_setup();
  	T = gsl_rng_default;
  	r = gsl_rng_alloc (T);
  	double sample;
		sample = gsl_ran_beta(r, alpha1, alpha2);
		std::cout<<"Sample "<<sample<<" "<<lower_bound<<" "<<upper_bound<<"\n";
		if(sample<upper_bound && sample>lower_bound)
			{return true;}
return false;	
}


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



unsigned statet::sum_outputs()
{
	unsigned sum=0;
	for(const auto &t: transitions)
	{
		if(t.countknown==true)
		{sum = sum + t.count;}

	}return sum;
}





std::vector<unsigned> MC::sample_parameter_distributions(std::vector<double> lower_bounds, std::vector<double> upper_bounds, unsigned N)
{
	
	const gsl_rng_type * T;
  	gsl_rng * r;
  	T = gsl_rng_default;
  	r = gsl_rng_alloc (T);
  	std::vector<unsigned> param_counts;
  	std::vector<unsigned> inv_param_counts;

  	unsigned kcount;
  	double prob;
  	bool all_constants_equal_one = true;
  	//sample parameters randomly
  	for(unsigned p_index=1; p_index<modelparams.size(); p_index++)
  		{	
  			modelparams[p_index].nom = 100*gsl_ran_beta(r,1,1);
  			modelparams[p_index].denom = 100;
  		}

 	std::vector< std::pair < statet, unsigned> > param_states;
	param_states = get_parameterised_states();

	for(auto s: param_states)
	{
		for(const auto t: s.first.transitions)
		{
			if(t.params.size()==1)
			{
				if(t.params[0].first==1)
				{
					param_counts[t.params[0].second] = param_counts[t.params[0].second] + t.count;
					inv_param_counts[t.params[0].second] = inv_param_counts[t.params[0].second] + s.first.sum_outputs()-t.count;
				}
				else
				{
					param_counts[t.params[0].second] = param_counts[t.params[0].second]+t.count;
					prob = t.params[0].first.nom/(double)t.params[0].first.denom;
					while(kcount<t.count)
					{	kcount = gsl_ran_binomial(r, prob, s.first.input);
						std::cout<<"sampling for kcount \n";}
					inv_param_counts[t.params[0].second] = inv_param_counts[t.params[0].second] + kcount - t.count;
				}
			}
			else
			{
				for(const auto p: t.params)
				{if(p.first!=1){all_constants_equal_one=false;}}

				if(all_constants_equal_one)
				{
					double probs[t.params.size()];
  					unsigned int sample[t.params.size()];
					//probs.resize(t.params.size());
					//sample.resize(t.params.size());
					for(unsigned i=0; i<t.params.size(); i++)
					{	probs[i] = t.params[i].first.nom/(double)t.params[i].first.denom;}

					gsl_ran_multinomial(r, t.params.size(), s.first.input, probs,sample);

					for(const auto q: sample)
					{
						std::cout<<q;
						std::cout<<"\n";
					}


				}



			}
		}
	}



for(const auto pc: param_counts)
{
	std::cout<<pc<<" HELLO ";
}

return param_counts;

}











