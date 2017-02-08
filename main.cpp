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
#include "distributions.h"
#define VERBOSE 1u


void help()
{
  std::cout<<"BaeVer \n";
}



int main(int argc, const char *argv[])
{

  try{
  tracet trace;
  MC model;

 model = get_simpleMC();
 model.callPrism();

  
//get data from model
  for(unsigned n=0; n<10; n++)
    { model.get_data(100);}
//do confidence calculation
model.confidencecalc(10);

}
catch(...)
{std::cout<<"exception caught at end of main \n";}


}

