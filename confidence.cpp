#include <vector>
#include <random>
#include <time.h>
#include "model.h"
#include <random>
#include <iostream>
#include <cassert>
#include "fraction.h"
#include "distributions.h"

void MC::reset_confidence()
{
  confidence.resize(modelparams.size());
  for(auto c: confidence)
  { 
    c.nom = 0;
    c.denom = 0;
  }
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


void MC::get_random_model_params(random_distribution &rd)
{
  for(unsigned i=1; i<modelparams.size(); i++)
  {
    modelparams[i].nom = 100*rd.beta(1,1);
    modelparams[i].denom = 100;
  }
}


void MC::sample_D_star(std::vector< std::pair < statet, unsigned> > &param_states, random_distribution &rd)
{
      std::vector<unsigned> param_counts;
    std::vector<unsigned> inv_param_counts;
    param_counts.resize(modelparams.size());
    inv_param_counts.resize(modelparams.size());
    unsigned kcount;
    double prob;
    bool all_constants_equal_one = true;


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
            kcount = rd.binomial(prob, s.first.input);
            std::cout<<"sampling for kcount \n"<<std::flush;}
          inv_param_counts[t.params[0].second] = inv_param_counts[t.params[0].second] + kcount - t.count;
        }
      }
      else
      {
        
        for(const auto p: t.params)
        { 
          if(p.first!=1)
            {all_constants_equal_one=false;}
        }

        if(all_constants_equal_one)
        {
          std::vector<double> probs(t.params.size());
          std::vector<unsigned> sample(t.params.size());  

          for(unsigned i=0; i<t.params.size(); i++)
          { probs[i] = modelparams[t.params[i].second].nom/(double)modelparams[t.params[i].second].denom;}
          rd.multinomial(t.params.size(), s.first.input, probs,sample);

          for(unsigned i=0; i<t.params.size(); i++)
          {
            param_counts[i] = param_counts[i] + sample[i];
            inv_param_counts[i] = param_counts[i] + s.first.input - sample[i];
          }

        }
        else
        {
          std::vector<double> probs1(t.params.size()+1);
          std::vector<unsigned> sample1(t.params.size()+1);  
          std::vector<double> probs2(t.params.size());
          std::vector<unsigned> sample2(t.params.size()); 
          probs1[t.params.size()+1] = 1;  
          for(unsigned i=0; i<t.params.size(); i++)
          {
            probs2[i] = modelparams[t.params[i].second].nom/(double)modelparams[t.params[i].second].denom;
            probs1[i] = 1 - probs2[i];
            probs1[t.params.size()+1] = probs1[t.params.size()+1] - t.params[i].first.nom/(double)t.params[i].first.denom;
          }
          
          rd.multinomial(t.params.size(), t.count, probs2,sample2);
          rd.multinomial(t.params.size()+1, s.first.input - t.count, probs1,sample1);

          for(unsigned i=0; i<t.params.size(); i++)
          {
            param_counts[i] = param_counts[i] + sample2[i];
            inv_param_counts[i] = param_counts[i] + sample1[i];
          }
        }
      }
    }
  }


std::cout<<"Param counts: ";
for(const auto pc: param_counts)
    std::cout<<pc<<" ";
std::cout<< "\nInv param counts: ";
for(const auto pc: inv_param_counts)
    std::cout<<pc<<" ";


parametercounts = param_counts;
inv_parametercounts = inv_param_counts;
std::cout<<"debug \n";

}


void MC::sample_params_update_conf(random_distribution &rd,
    std::vector<double> &lower_bounds, std::vector<double> &upper_bounds )
{
  std::cout<<"sample params and update confidence \n";
  double sample;
  for(unsigned i=0; i<modelparams.size(); i++)
  {
    sample = rd.beta(parametercounts[i],inv_parametercounts[i]);
    confidence[i].denom++;
    if(sample<upper_bounds[i] && sample>lower_bounds[i])
        confidence[i].nom++;  
  }
}

fractiont computeconfidence(std::vector<fractiont> conf_vector)
{
  fractiont result = 1;
  for(const auto c: conf_vector)
    {result = result * c;}

 return result; 
}


void MC::confidencecalc(
  unsigned num_samples, std::vector<double> lower_bounds, std::vector<double> upper_bounds)
{
  std::cout<<"confidence calc \n";

  random_distribution rd;
  rd.set_seed(0);  //we use the same seed so that we can reproduce experiments
  std::vector<fractiont> result(modelparams.size(), 0);
  std::vector< std::pair < statet, unsigned> > param_states;
  reset_confidence();

  param_states = get_parameterised_states();

  for(unsigned i=0; i<num_samples; i++)
  {
    get_random_model_params(rd);
    sample_D_star(param_states, rd);
    sample_params_update_conf(rd, lower_bounds, upper_bounds);
  }

}

