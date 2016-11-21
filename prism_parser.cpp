#include <string>
#include "prism_parser.h"
#include <iostream>
#include "model.h"
#include <assert.h>

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


void  MC::prism_find(std::string input)
{
  std::cout<<"PRISM parser \n";
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
}


bool result_bound_satisfied(unsigned i, double sample)
{
  


  return false;
}


bool MC::is_in_range(std::vector<double> &sample)
{
  assert(sample.size()==modelparams.size());
  bool found_bound=false;

  for(unsigned b=0; b<parameter_bounds.size(); b++)
  {
    found_bound=true;
    for(unsigned i=0; i< sample.size(); i++)
    {
      if(sample[i]<parameter_bounds[b][i].first || sample[i]>parameter_bounds[b][i].second)
        found_bound=false;
    }  
    if(found_bound==true)
      return result_bound_satisfied(b, sample);
  }

return false;
}



