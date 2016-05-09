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

 /*    
  try{
  tracet trace;
  std::random_device rd;
  std::default_random_engine generator(rd());
  MC model2, model3, model4;

  std::cout<<"Model 2 \n";
  model2 = get_parameterisedMC(4,10,10,10,10);
  model2.outputPRISM(std::cout);

  model2.PRISMsynthesis(f);
/*
  std::cout<<"Trace from model 2 \n";
  trace = gettrace(generator,model2,10);
  
  printtrace(trace);

 model2.check();
 std::cout<<"state split 1 \n";
 model3 = state_split1(model2);
// model3.outputMC(std::cout);
 model3.outputPRISM(std::cout);
 std::cout<<"state split 2 \n";
 model4 = state_split2(model3);
 model4.outputPRISM(std::cout);
  std::ofstream prismfile;
  prismfile.open("prismfile.pm");
  model4.outputPRISM(prismfile);
  

}
catch(...)
{std::cout<<"exception caught at end of main \n";}
*/
}

