#include <vector>
#include <cassert>
#include <random>
#include <algorithm>
#include <iostream>
#include "model.h"






int main(int argc, const char *argv[])
{



  
 try{
  tracet trace;
  std::random_device rd;
  std::default_random_engine generator(rd());
  MC model, model2, model3, model4;
//  model = get_ZeroConfMC(4,10,10,10,10);
  //trace = gettrace(generator,model,10);
  //printtrace(trace);


  std::cout<<"Model 2 \n";
  model2 = get_parameterisedMC(4,10,10,10,10);
//  model2.outputMC(std::cout);
  model2.outputPRISM(std::cout);
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
}
catch(...)
{std::cout<<"exception caught at end of main \n";}

}

