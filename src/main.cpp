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
#include <string>
#include <sstream>

#include "distributions.h"
#include "MC.h"
#include "MDP.h"
#define NUMBERSAMPLES 1000



void help()
{
  std::cout<<"BaeVer \n";
}

void output_header()
{
  std::cout<<"\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n\n"
            <<  "********************************* \n"
            << "         (\\.---.\/)\n"
            << "          /.-.-.\\ \n"
            << "         /| 0_0 |\\ \n"
            << "        |_`-(v)-'_|  \n"
            << "        \\`-._._.-'/      .-.\n"
            << "  -~-(((.`-\\_|_/-'.)))-~' <_ \n"
            << "         `.     .' \n"
            << "           `._.'\n"
            << "           BaeVer \n"
            << "  Bayesian Verification for DTMCs and MDPs   \n"
            << " elizabeth.polgreen@cs.ox.ac.uk \n"
            << " ******************************** \n\n";
         //   <<"for help file use command line option --help\n\n";

}




int main(int argc, const char *argv[])
{
  output_header();

  int verbose=0;
  int number_of_traces=1000;
  int trace_length=10;

  for(unsigned i=1; i<argc; i++)
   {
     if(std::string(argv[i])=="--verbose")
       { verbose=1;}
     else if(std::string(argv[i])=="--debug")
       { verbose=2;}
     else if(std::string(argv[i])=="--traces")
     {
       if(i+1<argc && isdigit(*argv[i+1]))
       {
         std::istringstream ss(argv[i+1]);
         if (!(ss >> number_of_traces))
           {std::cerr << "Invalid number " << argv[i+1] << '\n';}
         i++;
        }
      }
     else if(std::string(argv[i])=="--tracelength")
     {
       if(i+1<argc && isdigit(*argv[i+1]))
       {
         std::istringstream ss(argv[i+1]);
         if (!(ss >> trace_length))
           {std::cerr << "Invalid number " << argv[i+1] << '\n';}
         i++;
        }
      }
   }

  std::cout<<"Number of traces "<< number_of_traces<<"\n";
  std::cout<<"Trace length "<< trace_length<<"\n";


 //DO THE ACTUAL STUFF
try{


 // MC model = get_MC();

  MDP model = get_MDP();

  model.verbose = verbose;
  model.number_of_traces = number_of_traces;
  model.trace_length = trace_length;
  model.outputPrism(std::cout);
  model();


}
catch(...)
{std::cout<<"exception caught at end of main \n";}

std::cout<<"\n\n";
}

