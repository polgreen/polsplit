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

  try{
  tracet trace;
  MC model, model2;

  model = get_parameterisedMC(4,10,10,10,10);

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
model.confidencecalc(10);

}
catch(...)
{std::cout<<"exception caught at end of main \n";}


}

