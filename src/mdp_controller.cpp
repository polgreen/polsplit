/*
 * Interface and control header to MDP model
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "model_mdp.h"

void add_MDP_help() {

    std::cout << "Testing help for MDP \n"
            << "Testing help for MDP \n"
            << " Testing help for MDP\n";
}

void MDP_cmdvars::add_MDP_cmd_options(int argc, const char *argv[], int current, int* model_type) {
    int i = current;
    if (std::string(argv[i]) == "--MDP") {
        *model_type = 33586;
    } else if (std::string(argv[i]) == "--explicit_strategy") {       
        strategy_type_cap = 0;
    } else if (std::string(argv[i]) == "--first_strategy") {
       strategy_type_cap = 1;
    } else if (std::string(argv[i]) == "--random_strategy") {          
        strategy_type_cap = 2;        
    } else if (std::string(argv[i]) == "--no_strategy") {
        strategy_type_cap = 3;
    }
}

void MDP_cmdvars::display_MDP_cmd_options() {
    if (strategy_type_cap == 0) {
        std::cout << "Strategy: Explicit_strategy\n";
    } else if (strategy_type_cap == 1) {
        std::cout << "Strategy: first_strategy\n";

    } else if (strategy_type_cap == 2) {
        std::cout << "Strategy: random_strategy\n";

    } else if (strategy_type_cap == 3) {
        std::cout << "Strategy: no_strategy\n";
    }


}

void MDP_cmdvars::init_process(int verbose, int number_of_traces, int trace_length , long int_samples){
    
}

MDP_cmdvars get_MDP_cmdvars_instance() {
    MDP_cmdvars model;
    model.strategy_type_cap = 0;
    return model;

}
