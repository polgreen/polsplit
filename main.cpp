#include <vector>
#include <cassert>
#include <random>
#include <algorithm>
#include <iostream>
#include "model.h"
#include <fstream>
#include "pctl_tokenizer.h"
#include "pctl_parser.h"






int main(int argc, const char *argv[])
{
 std::vector<tokent> tokenseq;
 std::cout<< "Number of strings: "<<argc<<"\n";
 pctlformula f;

  if(argc==2)
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

  }

  
  try{
  tracet trace;

  MC model, model2, model3, model4;

  model = get_simpleMC();
  model.outputPRISM(std::cout);
  model.PRISMsynthesis(f);
 for(unsigned n=0; n<10; n++)
 {
  std::random_device rd;
  std::default_random_engine generator(rd());
  trace = gettrace(generator, model, 100);
  printtrace(trace);
  model.get_trace_counts(trace);
 }
 parameter_distributions(model);
 model.outputMC(std::cout);
/*
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
*/


  

}
catch(...)
{std::cout<<"exception caught at end of main \n";}

}

