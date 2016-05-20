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

std::vector<double> MC::parameter_distributions(std::vector<double> lower_bounds, std::vector<double> upper_bounds)
{
	std::cout<<"parameter distributions \n";
	std::vector< std::vector< std::pair < statet, unsigned> > > param_states;
	param_states = get_parameterised_states();
	std::vector<fractiont> result;
	std::vector<double> probabilities;
	unsigned placeholder;
	probabilities.resize(param_states.size());
	result.resize(param_states.size());
	if(lower_bounds.size()!=modelparams.size()||lower_bounds.size()!=upper_bounds.size()
		||param_states.size()!=modelparams.size())
	{
		std::cout<<"Error: lower_bounds must match upper_bounds and params in size \n";
		throw std::exception();
	}

	for(unsigned r=0; r<result.size(); r++)
	{result[r].one();}


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



std::vector<double> MC::sample_parameter_distributions(std::vector<double> lower_bounds, std::vector<double> upper_bounds, unsigned N)
{
	std::vector<statet> copy_states;
	const gsl_rng_type * T;
  	gsl_rng * r;
	unsigned input, n;
	gsl_rng_env_setup();
  	T = gsl_rng_default;
  	r = gsl_rng_alloc (T);
	std::vector< std::vector< std::pair < statet, unsigned> > > param_states;
							//param_states[i] = vector_of_states for parameter [i]. 
							//vector_of_states[i] = pair of {state and transition index} for a parameterised transition
	std::vector<fractiont> tracecounts;
	std::vector<double> probabilities;
	std::vector<unsigned> satisfying_samples;
	satisfying_samples.resize(modelparams.size());
	probabilities.resize(modelparams.size());
	tracecounts.resize(modelparams.size());
	
 	fractiont frac, frac_total;
 	unsigned count_total;
 	bool done=false;
 	unsigned loop_counter=0;
 	bool allcountsset=false;
	double prob, total_prob, sample;
	for(unsigned r=0; r<tracecounts.size(); r++)
	{tracecounts[r].one();
		satisfying_samples[r] = 0;}

	// missing data sample
	n=0;
	copy_states = states;
	while(n<N)
	{

		for(unsigned p_index=0; p_index<modelparams.size(); p_index++)
  		{	
  			modelparams[p_index].nom = 100*gsl_ran_beta(r,1,1);
  			modelparams[p_index].denom = 100;
  		}


  		do
  		{
  			for(auto &s: states)
  			{
  				if(s.inputknown==true && s.outputknown==false)
  				{
  					//std::cout<<"found S"<<s.ID<<" with unknown output \n";
  					count_total = s.input;
  					frac_total.one();
					for(auto t: s.transitions)
					{
					  if(t.countknown==true)
			 			{count_total=count_total - t.count;
				 		frac_total = frac_total - weighting(t,s);}
			 		}

			 		for(unsigned t_index=0; t_index<s.transitions.size(); t_index++)	
			 		{
			 			if(s.transitions[t_index].countknown==false)
			 			{	
			 				if(t_index==s.transitions.size()-1)
			 				{s.transitions[t_index].count=count_total;}	
			 				else{
			 					done=false; loop_counter=0;
			 				 while(done==false)
			 				 {
			 					frac = weighting(s.transitions[t_index],s)/frac_total;
			 			 		prob = frac.nom/(double)frac.denom;
			 			 		s.transitions[t_index].count = gsl_ran_binomial(r, prob, count_total);
			 			 		if((s.transitions[t_index].count<= states[s.transitions[t_index].successor].input)
			 			 			||states[s.transitions[t_index].successor].inputknown==false)
			 			 		{done=true;}
			 		 			count_total = count_total - s.transitions[t_index].count;
			 		 			frac_total = frac_total - frac*frac_total;
			 		 			loop_counter++;
			 		 			if(loop_counter>20){std::cout<<"Error: unable to generate valid hidden trace counts \n";
			 		 			throw std::exception();}
			 		 		 }
			 			 	}
			 			 	if(states[s.transitions[t_index].successor].inputknown==false)
				 			 {
				 		 		states[s.transitions[t_index].successor].input+=s.transitions[t_index].count;
			 		 			states[s.transitions[t_index].successor].inputknown=true;
			 			 	}
			 			 	 
			 		 		s.transitions[t_index].countknown=true;
			 			}
			 		}
			 	s.outputknown=true;
			 	}
  			} 			

  		//	std::cout<<"output MC with hidden data \n";
  	//		outputMC(std::cout);

			allcountsset=true;	
  			for(auto &s: states)
  			{
  		//		std::cout<<"State "<<s.ID<<" input = "<<s.input<<"\n";
  				if(s.inputknown==false || s.outputknown==false)
  				{
  					allcountsset=false;
  				}
  			}	

  		}while(allcountsset==false);	

  		param_states = get_parameterised_states();
  		for(unsigned p_index=0; p_index<param_states.size(); p_index++)
		{
			//get trace counts for each parameter
			for(const auto s: param_states[p_index])
			{tracecounts[p_index].nom = tracecounts[p_index].nom + s.first.transitions[s.second].count;
			tracecounts[p_index].denom += s.first.input - s.first.transitions[s.second].count;}
		}
		for(unsigned r_index=0; r_index<tracecounts.size(); r_index++)
		{
			sample = gsl_ran_beta(r, tracecounts[r_index].nom, tracecounts[r_index].denom);
			std::cout<<" sample "<<sample;
			if(sample>lower_bounds[r_index] && sample<upper_bounds[r_index])
			{satisfying_samples[r_index]++;}
		}
		states = copy_states;
  		n++;
  	}
  	std::cout<<"\n";
  	for(const auto &sat: satisfying_samples)
	{
		std::cout<<" "<<sat;
	}


  return probabilities;
  }		









