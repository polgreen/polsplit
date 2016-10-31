 /*****************************\
          (\.---./)    
           /.-.-.\
          /| 0_0 |\
         |_`-(v)-'_|
         \`-._._.-'/      .-.
   -~-(((.`-\_|_/-'.)))-~' <_
          `.     .'
            `._.'
          BaeVer
 Bayesian Verification for DTMCs     
   -----~--~---~~~----~-`.-;~
   elizabeth.polgreen@cs.ox.ac.uk
\********************************/



#include <vector>
#include <cassert>
#include <random>
#include <algorithm>
#include <iostream>
#include <fstream>

#include "model.h"
#include "pctl_tokenizer.h"
#include "pctl_parser.h"
#include "distributions.h"



int main(int argc, const char *argv[])
{
 // parse pctl formula
  std::vector<tokent> tokenseq;
  std::cout<< "Number of strings: "<<argc<<"\n";
  pctlformula f;

  if(argc==2)
  {
    tokenseq = pctl_tokenizer(argv[1]);
    outputtoken(tokenseq);

   try
   {
      f = parse_top(tokenseq);
      std::cout<<"\n";
      output(f);
      std::cout<<"\n";
   }
   catch(std::exception& ex)
        {std::cout << "parsing failed\n";}
  }

  //get region for parameter from parameter synthesis tool
  std::vector<double> lower_bounds;
  std::vector<double> upper_bounds;
  try{
  tracet trace;
  MC model, model2;

  model = get_parameterisedMC(4,10,10,10,10);

  lower_bounds.resize(model.modelparams.size());
  upper_bounds.resize(model.modelparams.size());

  for(unsigned i=0; i<lower_bounds.size(); i++)
  {
    lower_bounds[i] = 0;
    upper_bounds[i] = 0.5;
  }

  model.PRISMsynthesis(f);
  model.outputPRISM(std::cout);
  
//get data from model
  for(unsigned n=0; n<10; n++)
  {
   trace = gettrace(model, 100);
    model.get_trace_counts(trace);
   }
std::cout<<"Confidence calc\n";
//do confidence calculation
model.confidencecalc(10,lower_bounds, upper_bounds);

}
catch(...)
{std::cout<<"exception caught at end of main \n";}

}





 
 /* 
 std::vector<tokent> tokenseq;
 std::cout<< "Number of strings: "<<argc<<"\n";
 pctlformula f;
   std::random_device rd;
/*  if(argc==2)
  {
    tokenseq = pctl_tokenizer(argv[1]);
    outputtoken(tokenseq);

   try{
      f = parse_top(tokenseq);
      std::cout<<"\n";
      output(f);
      std::cout<<"\n";
        }
   catch(std::exception& ex)
        {std::cout << "parsing failed\n";}

  }*/

  /*
  try{
  tracet trace;
  MC model, model2, model3, model4;

  model = get_simpleMC();
 // model.outputPRISM(std::cout);
 // model.PRISMsynthesis(f);
 for(unsigned n=0; n<100; n++)
 {
  trace = gettrace(model, 100);
//  printtrace(trace);
  model.get_trace_counts(trace);
 }
 parameter_distributions(model);
 model.outputMC(std::cout);

  std::cout<<"Model 2 \n";
  model2 = get_parameterisedMC(4,10,10,10,10);

  model3 = state_split1(model2);
  model4 = state_split2(model3);
  //model2.PRISMsynthesis(f);

  std::cout<<"Trace from model 2 \n";
  trace = gettrace(generator,model4,100);
  printtrace(trace);
  model4.get_trace_counts(trace);
   model4.outputMC(std::cout); 
  parameter_distributions(model4);


}
catch(...)
{std::cout<<"exception caught at end of main \n";}

}
*/
