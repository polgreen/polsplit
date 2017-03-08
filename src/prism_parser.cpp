#include <string>
#include <iostream>
#include <assert.h>
#include "MC.h"
#include "MDP.h"



std::pair<double, double> parse_bound(std::string &bound)
{
  double lower, upper;
  std::pair<double, double> result;
  std::string::size_type sz;

  lower = std::stod(bound, &sz);
  upper = std::stod(bound.substr(sz+1));

  result.first = lower;
  result.second = upper;
  return result;
}


void  MC::prism_find(std::string& input)
{
  if(verbose>1)
    std::cout<<"input to bound parsing: "<<input<<"\n";
  std::string res = "Result:";
  std::string substring, bound, result;
  std::string LP = "(";
  std::string RP = ")";
  std::string SLP = "[";
  std::string SRP = "]";
  std::string comma = ",";
  std::string truestring = " true";
  std::string falsestring = "false"; 
  std::string newline = "\n";

  std::size_t found = input.find(res);
  if(found!=std::string::npos)
    {substring = input.substr(found);}  
   else  
    {std::cout<<"Could not find Result in prism output \n";}

  std::vector<std::pair<double, double > > bounds;

  unsigned i=0, j=0;
  std::string::iterator pos =substring.begin();
  while(pos!=substring.end())
  {    
    if(*pos=='[') //get bound
    {
      pos++;
      while(*pos!=']') //look for end of bound
      {
        bound+= *pos;
        pos++;
      } 
      bounds.push_back(parse_bound(bound));
      bound.clear();
      i++;
    }
    else
    {
      if(*pos==')') //found end of bounds
      {
        pos++;
        while(*pos != '\n')
        {
          result+=*pos;
          pos++;
        }  
        parameter_results.push_back(result);
        result.clear();
        parameter_bounds.push_back(bounds);
        bounds.clear();
        j++; 
      } 
      pos++;
    }
  }
  if(verbose>1)
  {
    for(unsigned i=0; i<parameter_bounds.size(); i++)
    {
      for(unsigned j=0; j<parameter_bounds[i].size(); j++)
        {std::cout<<"bound "<<i<<": "<<parameter_bounds[i][j].first
                  <<" -> "<<parameter_bounds[i][j].second;}
        std::cout<<" : result "<< parameter_results[i]<<"\n";
    }
  }
}

bool MC::result_bound_satisfied(unsigned i, std::vector<double>& sample)
{
  std::string truestr("true");
  std::string falsestr("false");
  std::size_t found = parameter_results[i].find(truestr);
  if(found != std::string::npos)
    return true;
  found = parameter_results[i].find(falsestr);
  if(found!=std::string::npos)
    return false;

  std::cout<<"ERROR: feasible sets expressed as polynomials not supported \n";
  throw std::exception();
}

bool MC::is_in_range(std::vector<double> &sample, bool update_param_conf)
{

  assert(sample.size()==modelparams.size()-1);
  bool in_range=false;
  bool all_params_ok=true;


  for(int b=0; b<parameter_bounds.size(); b++)
  {
    in_range=false;
    for(int s=0; s<sample.size(); s++)
    {
      in_range=true;
      if(sample[s]>=parameter_bounds[b][s].first && sample[s]<=parameter_bounds[b][s].second)
        {std::cout<<"sample  p"<<s<<"="<<sample[s]<<", in bound "<<b<<": "<<parameter_bounds[b][s].first;
        std::cout<<" -> "<<parameter_bounds[b][s].second<<std::endl;}
      else
        in_range=false;
    }
    if(in_range==true)
    {
      if(result_bound_satisfied(b, sample))
       {
        if(verbose > 1)
            std::cout<<" RESULT = true \n";
        return true;
       }
      else
      {
        if(verbose>1)
            std::cout<<"RESULT = false\n";
        return false;
      }
     }
  }
  return false;
}

/*  for(unsigned i=0; i<sample.size(); i++)
  {
    in_range=false;
    if(verbose>1)
      std::cout<<"Check sample P"<<i+1<<" = "<<sample[i]<<" is in bounds: ";
    for(unsigned b=0; b<parameter_bounds.size(); b++)
    {
      if(sample[i]>=parameter_bounds[b][i].first && sample[i]<=parameter_bounds[b][i].second)
      {
        if(verbose>1)
          {std::cout<<"parameter in range "<< parameter_bounds[b][i].first<<" ->"
                  <<parameter_bounds[b][i].second<<", ";}
        in_range=true;
        if(result_bound_satisfied(b, sample))
         {
          if(update_param_conf)
              confidence[i].nom++; //update individual param confidences
          if(verbose > 1)
              std::cout<<" RESULT = true \n";
         }
        else
        {
          if(verbose>1)
              std::cout<<"RESULT = false\n";all_params_ok=false;}
       }
    }
    if(in_range==false)
      { all_params_ok=false;}
    else if(update_param_conf)
    {confidence[i].denom++;}
  }
return all_params_ok;
}*/


void  MDP::prism_find(std::string& input)
{
  if(verbose>1)
    std::cout<<"input to bound parsing: "<<input<<"\n";
  std::string res = "Result:";
  std::string substring, bound, result;
  std::string LP = "(";
  std::string RP = ")";
  std::string SLP = "[";
  std::string SRP = "]";
  std::string comma = ",";
  std::string truestring = " true";
  std::string falsestring = "false";
  std::string newline = "\n";

  std::size_t found = input.find(res);
  if(found!=std::string::npos)
    {substring = input.substr(found);}
   else
    {std::cout<<"Could not find Result in prism output \n";}

  std::vector<std::pair<double, double > > bounds;

  unsigned i=0, j=0;
  std::string::iterator pos =substring.begin();
  while(pos!=substring.end())
  {
    if(*pos=='[') //get bound
    {
      pos++;
      while(*pos!=']') //look for end of bound
      {
        bound+= *pos;
        pos++;
      }
      bounds.push_back(parse_bound(bound));
      bound.clear();
      i++;
    }
    else
    {
      if(*pos==')') //found end of bounds
      {
        pos++;
        while(*pos != '\n')
        {
          result+=*pos;
          pos++;
        }
        parameter_results.push_back(result);
        result.clear();
        parameter_bounds.push_back(bounds);
        bounds.clear();
        j++;
      }
      pos++;
    }
  }
  if(verbose>1)
  {
    for(unsigned i=0; i<parameter_bounds.size(); i++)
    {
      for(unsigned j=0; j<parameter_bounds[i].size(); j++)
        {std::cout<<"bound "<<i<<": "<<parameter_bounds[i][j].first
                  <<" -> "<<parameter_bounds[i][j].second;}
        std::cout<<" : result "<< parameter_results[i]<<"\n";
    }
  }
}

bool MDP::result_bound_satisfied(unsigned i, std::vector<double>& sample)
{
  std::string truestr("true");
  std::string falsestr("false");
  std::size_t found = parameter_results[i].find(truestr);
  if(found != std::string::npos)
    return true;
  found = parameter_results[i].find(falsestr);
  if(found!=std::string::npos)
    return false;

  std::cout<<"ERROR: feasible sets expressed as polynomials not supported \n";
  throw std::exception();
}

bool MDP::is_in_range(std::vector<double> &sample, bool update_param_conf)
{
  assert(sample.size()==modelparams.size()-1);
  bool in_range=false;
  bool all_params_ok=true;


  for(int b=0; b<parameter_bounds.size(); b++)
  {
    in_range=false;
    for(int s=0; s<sample.size(); s++)
    {
      in_range=true;
      if(sample[s]>=parameter_bounds[b][s].first && sample[s]<=parameter_bounds[b][s].second)
        {std::cout<<"sample  p"<<s<<"="<<sample[s]<<", in bound "<<b<<" "<<parameter_bounds[b][s].first;
                  std::cout<<" "<<parameter_bounds[b][s].second<<std::endl;
        }
      else
        in_range=false;
    }
    if(in_range==true)
    {
      if(result_bound_satisfied(b, sample))
       {
        if(verbose > 1)
            std::cout<<" RESULT = true \n";
        return true;
       }
      else
      {
        if(verbose>1)
            std::cout<<"RESULT = false\n";
        return false;
      }
     }
  }
  return false;
}

