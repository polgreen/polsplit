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
#define VERSION 0001
#include <vector>
#include <cassert>
#include <random>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>

#include "distributions.h"
#include "MC.h"
#include "MDP.h"


void help()
{
  std::cout
      << "BaeVer is the experimental implementation for paper DOI:10.1007/978-3-319-43425-4_3. \n"
      << "It runs the experiments for two simple models, one Markov Chain and one Markov Decision Process. \n"
      << " The models are input in the file models.cpp.\n"
      << "For installation instructions please see https://github.com/polgreen/BaeVer\n"
      << " The following command line arguments are available:\n"
      << "--verbosity N increases the verbosity of the output to N\n"
      << "--traces N sets the number of traces to N, default is 1000\n"
      << "--tracelength N sets the length of each trace to N transitions, default is 1000\n"
      << "--integrationsamples N sets the number of samples used by"
      << " monte-carlo integration to N, default is 10,000\n"
      << "--MDP N runs the MDP model N, default is to run the MC model\n"
      << "--explicit_strategy explicitly evaluates all memoryless strategies \n"
      << "--random_strategy picks a random memoryless strategy \n"
      << "--no_strategy picks the first action at each state \n"
      <<"--first_strategy uses explicit synthesis to synthesise an initial strategy, and then reuses this strategy \n"
      << "--batch N runs the experiment N times \n\n";
}

void output_header()
{
  std::cout << "\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n\n"
      << "********************************* \n" << "         (\\.---.\/)\n"
      << "          /.-.-.\\ \n" << "         /| 0_0 |\\ \n"
      << "        |_`-(v)-'_|  \n" << "        \\`-._._.-'/      .-.\n"
      << "  -~-(((.`-\\_|_/-'.)))-~' <_ \n" << "         `.     .' \n"
      << "           `._.'\n" << "           BaeVer \n"
      << "  Bayesian Verification for DTMCs and MDPs   \n"
      << " elizabeth.polgreen@cs.ox.ac.uk \n"
      << " ******************************** \n\n"
      << "for help file use command line option --help\n\n";

}

int main(int argc, const char *argv[])
{
  output_header();

  //default values
  int verbose = 0;
  int number_of_traces = 1000;
  int trace_length = 10;
  int num_int_samples = 10000;
  int strategy = 0;
  int model_num = 0;
  int batch=1;
  bool modelMDP = false;

  for (unsigned i = 1; i < argc; i++)
  {
    if (std::string(argv[i]) == "--verbosity")
    {
      if (i + 1 < argc && isdigit(*argv[i + 1]))
      {
        std::istringstream ss(argv[i + 1]);
        if (!(ss >> verbose))
        {
          std::cerr << "Invalid number " << argv[i + 1] << '\n';
        }
        i++;
      }
    }
    else if (std::string(argv[i]) == "--traces")
    {
      if (i + 1 < argc && isdigit(*argv[i + 1]))
      {
        std::istringstream ss(argv[i + 1]);
        if (!(ss >> number_of_traces))
        {
          std::cerr << "Invalid number " << argv[i + 1] << '\n';
        }
        i++;
      }
    }
    else if (std::string(argv[i]) == "--tracelength")
    {
      if (i + 1 < argc && isdigit(*argv[i + 1]))
      {
        std::istringstream ss(argv[i + 1]);
        if (!(ss >> trace_length))
        {
          std::cerr << "Invalid number " << argv[i + 1] << '\n';
        }
        i++;
      }
    }
    else if (std::string(argv[i]) == "--batch")
    {
      if (i + 1 < argc && isdigit(*argv[i + 1]))
      {
        std::istringstream ss(argv[i + 1]);
        if (!(ss >> batch))
        {
          std::cerr << "Invalid number " << argv[i + 1] << '\n';
        }
        i++;
      }
    }
    else if (std::string(argv[i]) == "--integrationsamples")
    {
      if (i + 1 < argc && isdigit(*argv[i + 1]))
      {
        std::istringstream ss(argv[i + 1]);
        if (!(ss >> num_int_samples))
        {
          std::cerr << "Invalid number " << argv[i + 1] << '\n';
        }
        i++;
      }
    }
    else if (std::string(argv[i]) == "--MDP")
    {
      if (i + 1 < argc && isdigit(*argv[i + 1]))
      {
        std::istringstream ss(argv[i + 1]);
        if (!(ss >> model_num))
        {
          std::cerr << "Invalid number " << argv[i + 1] << '\n';
        }
        i++;
      }
      modelMDP = true;
    }
    else if (std::string(argv[i]) == "--explicit_strategy")
    {
      strategy = EXPLICIT;
    }
    else if (std::string(argv[i]) == "--first_strategy")
    {
      strategy = FIRST;
    }
    else if (std::string(argv[i]) == "--random_strategy")
    {
      strategy = RANDOM;
    }
    else if (std::string(argv[i]) == "--no_strategy")
    {
      strategy = NONE;
    }
    else if (std::string(argv[i]) == "--help")
    {
      help();
      return 0;
    }
  }



  std::ofstream results;
  std::string resultfilename=("Results_strat"+std::to_string(strategy)+"_tr"+std::to_string(number_of_traces)+"_l"+std::to_string(trace_length)+".csv");

  results.open(resultfilename, std::ofstream::out | std::ofstream::app);
  //CSV columns: model, model num, trace num, trace length, integration samples, strategy choice, confidence

  std::cout << "Number of traces " << number_of_traces << "\n";
  std::cout << "Trace length " << trace_length << "\n";

  //DO THE ACTUAL STUFF
  try
  {

    random_distribution rd;
    rd.set_seed(0);


    fractiont confidence;
    if (modelMDP)
    {
      MDP model = get_MDP(model_num);

      std::cout << "Model = simple Markov decision process \n";

      model.verbose = verbose;
      model.number_of_traces = number_of_traces;
      model.trace_length = trace_length;
      model.num_int_samples = num_int_samples;
      model.strategy_type = strategy;
      for(int i=0; i<batch; i++)
      {
        results<<"MDP , "<<model_num<<" , ";
              for(const auto &p: model.modelparams)
                results<<fraction_to_double(p)<<" , ";
              results<<number_of_traces<<" , "<<trace_length<<" , "<<num_int_samples<<" , "<<strategy<<" , ";
        confidence = model(rd, (i==0));
        results<<fraction_to_double(confidence)<<" , ";
           std::cout << "\nFinal confidence: " << confidence.nom << " / "
               << confidence.denom<<"," ;
           for(int i=1; i<model.modelparams.size(); i++)
           {
             results<<model.prior_a1[i]<<" , "<<model.prior_a2[i]<<" , ";
           }
           results<<std::endl;
      }

    }
    else
    {
      std::cout << "Model = simple Markov chain \n";
      MC model = get_MC();
      results<<"MC , "<<0<<" , ";
      for(const auto &p: model.modelparams)
        results<<fraction_to_double(p)<<" , ";
      results<<number_of_traces<<", "<<trace_length<<" , "<<num_int_samples<<" , "<<0<<" , ";

      model.verbose = verbose;
      model.number_of_traces = number_of_traces;
      model.trace_length = trace_length;
      model.num_int_samples = num_int_samples;
      for(int i=0; i<batch; i++)
      {
        results<<"MDP , "<<model_num<<" , ";
                      for(const auto &p: model.modelparams)
                        results<<fraction_to_double(p)<<" , ";
                      results<<number_of_traces<<" , "<<trace_length<<" , "<<num_int_samples<<" , "<<strategy<<" , ";
                confidence = model(rd);
                results<<fraction_to_double(confidence)<<std::endl;
                   std::cout << "\nFinal confidence: " << confidence.nom << " / "
                       << confidence.denom << std::endl;
      }

    }

  } catch (...)
  {
    std::cout << "exception caught at end of main \n";
  }

  std::cout << "\n\n";
  return 0;
}

