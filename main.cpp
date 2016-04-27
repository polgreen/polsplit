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
  MC model;
  model = get_ZeroConfMC(4, 10,10,40,10);
  trace = gettrace(generator,model,10);
  printtrace(trace);

}

