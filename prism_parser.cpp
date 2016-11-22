#include <string>
#include "prism_parser.h"
#include <iostream>
#include "model.h"
#include <assert.h>

enum mathoperator {PLUS, MINUS, DIVIDE, MULTIPLY };

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


bool MC::function_satisfied(std::string &bound, std::vector<double> &sample )
{
  std::string::iterator pos =bound.begin();
  char letter = 'a';
  double nominator = 0;
  double denominator=0;
  bool added_to_result=false;
  mathoperator mop = PLUS;
  bool first=false;

  std::cout<<"Parsing of PRISM output expression not yet supported:\n ";
  std::cout<<bound<<" is expression\n";
/*
  while(pos!=bound.end())
  {
    for(unsigned i=0; i<modelparams.size(); i++)
    {
      if(*pos = static_cast<char>(letter + i))
         {result  sample[i];
          added_to_result=true;}
    }
    if(!added_to_result && isdigit(*pos))
      result = result * atoi(*pos);
    else if ()
  }*/
return false;
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

  return function_satisfied(parameter_results[i], sample);
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



