#include <vector>
#include <random>
#include <time.h>
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


void MC::sample_transition_counts()
{
	
	const gsl_rng_type * T;
  	gsl_rng * r;
  	T = gsl_rng_default;
  	r = gsl_rng_alloc (T);
  	

  	std::vector<unsigned> param_counts;
  	std::vector<unsigned> inv_param_counts;
  	param_counts.resize(modelparams.size());
  	inv_param_counts.resize(modelparams.size());
  	unsigned kcount;
  	double prob;
  	bool all_constants_equal_one = true;
  	//sample parameters randomly
  	for(unsigned p_index=1; p_index<modelparams.size(); p_index++)
  		{	
  			modelparams[p_index].nom = 100*gsl_ran_beta(r,1,1);
  			modelparams[p_index].denom = 100;
  			param_counts[p_index] = 0;
  			inv_param_counts [p_index] = 0;

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
					{	
						kcount = gsl_ran_binomial(r, prob, s.first.input);
						std::cout<<"sampling for kcount \n"<<std::flush;}
					inv_param_counts[t.params[0].second] = inv_param_counts[t.params[0].second] + kcount - t.count;
				}
			}
			else
			{
				
				for(const auto p: t.params)
				{if(p.first!=1){all_constants_equal_one=false;
					}}

				if(all_constants_equal_one)
				{
			
					double probs[t.params.size()];
  					unsigned int sample[t.params.size()];
					//probs.resize(t.params.size());
					//sample.resize(t.params.size());
					for(unsigned i=0; i<t.params.size(); i++)
					{	probs[i] = modelparams[t.params[i].second].nom/(double)modelparams[t.params[i].second].denom;}
					gsl_ran_multinomial(r, t.params.size(), s.first.input, probs,sample);

					for(unsigned i=0; i<t.params.size(); i++)
					{
						param_counts[i] = param_counts[i] + sample[i];
						inv_param_counts[i] = param_counts[i] + s.first.input - sample[i];
					}

				}
				else
				{
			
					double probs1 [t.params.size()+1];
					unsigned int sample1[t.params.size()+1];
					double probs2[t.params.size()];
					unsigned int sample2[t.params.size()];
					probs1[t.params.size()+1] = 1;	
					for(unsigned i=0; i<t.params.size(); i++)
					{
						probs2[i] = modelparams[t.params[i].second].nom/(double)modelparams[t.params[i].second].denom;
						probs1[i] = 1 - probs2[i];
						probs1[t.params.size()+1] = probs1[t.params.size()+1] - t.params[i].first.nom/(double)t.params[i].first.denom;
					}
					
					gsl_ran_multinomial(r, t.params.size(), t.count, probs2,sample2);
					gsl_ran_multinomial(r, t.params.size()+1, s.first.input - t.count, probs1,sample1);

					for(unsigned i=0; i<t.params.size(); i++)
					{
						param_counts[i] = param_counts[i] + sample2[i];
						inv_param_counts[i] = param_counts[i] + sample1[i];
					}

		
				}



			}
		}
	}


	for(const auto pc: param_counts)
	std::cout<<"Param count: "<<pc<<"\n";

parametercounts = param_counts;
inv_parametercounts = inv_param_counts;


}

std::vector<fractiont> MC::confidencecalc(unsigned num_samples, std::vector<double> lower_bounds, std::vector<double> upper_bounds)
{
	const gsl_rng_type * T;
  	gsl_rng * r;
	gsl_rng_env_setup();
	T = gsl_rng_default;
  	r = gsl_rng_alloc (T);

	std::vector<fractiont> result(modelparams.size(), 0);
//	result.resize(modelparams.size());
//	for(auto r: result)
//		{r=0;}
	for(unsigned i=0; i<num_samples; i++)
	{
		sample_transition_counts();
		for(unsigned j=0; j<parametercounts.size(); j++)
		{
			result[j].denom = result[j].denom+1;
  			double sample;
			sample = gsl_ran_beta(r, parametercounts[j], inv_parametercounts[j]);
			std::cout<<"Sample: "<<sample<<"\n";
			//std::cout<<"sample lower bound"<<lower_bounds[j]<<" and upper "<<upper_bounds[j]<<"\n";
			if(sample<upper_bounds[j] && sample>lower_bounds[j])
			{result[j].nom = result[j].nom + 1;}
	
		}

	}
	std::cout<<"\n";
for(const auto r: result)
{
	std::cout<<r.nom<<"/"<<r.denom<<" ";
}
return result;
}








