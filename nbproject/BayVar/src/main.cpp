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

#include "model.h"
#include "distributions.h"
#define NUMBERSAMPLES 1000

void help() {
    std::cout<<"BaeVer is the experimental implementation for paper DOI:10.1007/978-3-319-43425-4_3. \n"
          <<"It runs the experiments for two simple models, one Markov Chain and one Markov Decision Process. \n"
          <<" The models are input in the file models.cpp.\n"
          <<"For installation instructions please see https://github.com/polgreen/BaeVer\n"
          <<" The following command line arguments are available:\n"
          <<"--verbose increases the verbosity of the output\n"
          <<"--debug gives the maximally verbose output\n"
          <<"--traces N sets the number of traces to N, default is 1000\n"
          <<"--tracelength N sets the length of each trace to N transitions, default is 1000\n"
          <<"--intsamples N sets the number of samples used by"
          <<" monte-carlo integration to N, default is 10,000\n"
          <<"--MDP runs the simple MDP model, default is to run the MC model\n\n";
}

void output_header() {
    std::cout << "\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n\n"
            << "********************************************* \n"
            << "         (\\.---.\\)\n"
            << "          /.-.-.\\ \n"
            << "         /| 0_0 |\\ \n"
            << "        |_`-(v)-'_|  \n"
            << "        \\`-._._.-'/      .-.\n"
            << "  -~-(((.`-\\_|_/-'.)))-~' <_ \n"
            << "         `.     .' \n"
            << "           `._.'\n"
            << "           BaeVer \n"
            << " Bayesian Verification for DTMCs and MDPs \n"
            << " elizabeth.polgreen@cs.ox.ac.uk \n"
            << " ******************************************* \n\n";
    //   <<"for help file use command line option --help\n\n";

}

int main(int argc, const char *argv[]) {
    output_header();

    int verbose = 0;
    int number_of_traces = 10;
    int trace_length = 10;
    long int_samples = 10000; //Integration Samples
    bool mdlMDP = false; // Switch to MDP Verification

    for (unsigned i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "--verbose") {
            verbose = 1;
        } else if (std::string(argv[i]) == "--debug") {
            verbose = 2;
        } else if (std::string(argv[i]) == "--traces") {
            if (i + 1 < argc && isdigit(*argv[i + 1])) {
                std::istringstream ss(argv[i + 1]);
                if (!(ss >> number_of_traces)) {
                    std::cerr << "Invalid number " << argv[i + 1] << '\n';
                }
                i++;
            }
        } else if (std::string(argv[i]) == "--tracelength") {
            if (i + 1 < argc && isdigit(*argv[i + 1])) {
                std::istringstream ss(argv[i + 1]);
                if (!(ss >> trace_length)) {
                    std::cerr << "Invalid number " << argv[i + 1] << '\n';
                }
                i++;
            }
        } else if (std::string(argv[i]) == "--intsamples") {
            if (i + 1 < argc && isdigit(*argv[i + 1])) {
                std::istringstream ss(argv[i + 1]);
                if (!(ss >> int_samples)) {
                    std::cerr << "Invalid number " << argv[i + 1] << '\n';
                }
                i++;
            }
        } else if (std::string(argv[i]) == "--MDP") {
            mdlMDP = true;
        }
    }

    std::cout << "Number of traces " << number_of_traces << "\n";
    std::cout << "Trace length " << trace_length << "\n";
    std::cout << "Number of Integration Samples " << int_samples << "\n";

    //DO THE ACTUAL STUFF

    try {

        if (mdlMDP) {
            std::cout << "Model = simple Markov decision process \n\n";
        } else {
            std::cout << "Model = simple Markov chain \n\n";
            MC model = get_simpleMC();
            model.verbose = verbose;
            model.number_of_traces = number_of_traces;
            model.trace_length = trace_length;

            model.callPrism();
            std::cout << "\n";

            random_distribution rd;
            rd.set_seed(0);
            //get data from model
            for (unsigned n = 0; n < model.number_of_traces; n++) {
                model.get_data(model.trace_length, rd);
            }
            //do confidence calculation
            model.confidencecalc(NUMBERSAMPLES);
        }
    } catch (...) {
        std::cout << "exception caught at end of main \n";
    }

    std::cout << "\n\n";
}

