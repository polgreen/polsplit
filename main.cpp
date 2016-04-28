#include <vector>
#include <cassert>
#include <random>
#include <algorithm>
#include <iostream>
#include "model.h"






int main(int argc, const char *argv[])
{
 
  tracet trace;
  std::random_device rd;
  std::default_random_engine generator(rd());
  MC model, model2, model3;
//  model = get_ZeroConfMC(4,10,10,10,10);
  //trace = gettrace(generator,model,10);
  //printtrace(trace);


  std::cout<<"Model 2 \n";
  model2 = get_parameterisedMC(4,10,10,10,10);
  model2.outputMC(std::cout);
  std::cout<<"state split \n";
  model3 = state_split(model2);
  model3.outputMC(std::cout);
  model3.outputPRISM(std::cout);



}

