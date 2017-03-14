/*
 * Interface and control header to MDP model
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "model.h"
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
    } else if (std::string(argv[i]) == "--random_finite_memory") {
        strategy_type_cap = 3;
    } else if (std::string(argv[i]) == "--no_strategy") {
        strategy_type_cap = 4;
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

void MDP_cmdvars::init_process(int verbose, int number_of_traces, int trace_length, long int_samples) {
    MDP model = get_MDP_instance();
    model.verbose = verbose;
    model.number_of_traces = number_of_traces;
    model.trace_length = trace_length;
    model.int_samples = int_samples;
    model.strategy_type = strategy_type_cap;
    model.prepModel();
    model.callPrism();
    random_distribution rd = initRndDistribution();
    for (unsigned n = 0; n < model.number_of_traces; n++) {
        model.synthStrategy(rd);
        MC inducd_model = induceMarkovChain(model);
        inducd_model.get_data(rd);
        inducd_model.confidencecalc();
        // std::cout << "overall confidence " << inducd_model.overall_confidence.nom << "/" << inducd_model.overall_confidence.denom << "\n";
        model.overall_confidence = inducd_model.overall_confidence;
        for (int i = 1; i < model.modelparams.size(); i++) {
            model.beta_prior_param1[i] += inducd_model.parametercounts[i];
            model.beta_prior_param2[i] += inducd_model.inv_parametercounts[i];
            if (verbose > 0)
                std::cout << "updated prior for p" << i << " " << model.beta_prior_param1[i] << " " << model.beta_prior_param2[i] << std::endl;
        }
    }
    //model.overall_confidence.avg(model.number_of_traces);
    model.displayConfidence();
}

MDP_cmdvars get_MDP_cmdvars_instance() {
    MDP_cmdvars model;
    model.strategy_type_cap = 0;
    return model;

}

void MDP::outputPRISM(std::ostream &out) {
    //output the prism file for an MDP
    char letter = 'a';
    int action = 0;
    out << "mdp \n\n";

    for (unsigned p_index = 1; p_index < modelparams.size(); p_index++) {
        out << "const double ";
        out << static_cast<char> (letter + p_index - 1) << ";\n";
    }
    out << "\n\nmodule testmdp \n";
    out << "\n // local state \n s: [0.." << states.size() - 1 << "] init "
            << get_init_state().ID;
    out << ";\n \n";

    for (auto & s : states) {

        for (auto &a : s.actions) {
            bool first_t = true;
            out << "[act" << action << "] s=" << s.ID << " -> ";
            action++;
            for (auto & t : a.first) {
                if (first_t) {
                    first_t = false;
                } else {
                    out << " + ";
                }
                bool first_p = true;
                switch (t.type) {
                    case CONST:
                        t.prob.output(out);
                        out << " : (s'=" << t.successor << ") ";
                        break;
                    case FUNCTION:
                        for (auto & p : t.params) {
                            if (first_p) {
                                first_p = false;
                            } else {
                                out << " + ";
                            }
                            p.first.output(out);
                            out << "*";
                            if (p.second > 0) {
                                out << static_cast<char> (letter + p.second - 1);
                            }
                        }
                        out << " : (s'=" << t.successor << ")";
                        break;
                    case REMAINDER:
                        out << "(1";
                        for (auto & t2 : a.first)
                            switch (t2.type) {
                                case CONST:
                                    out << "-";
                                    t2.prob.output(out);
                                    break;
                                case FUNCTION:
                                    for (auto & p2 : t2.params) {
                                        out << "-";
                                        p2.first.output(out);
                                        out << "*";
                                        if (p2.second > 0) {
                                            out << static_cast<char> (letter + p2.second - 1);
                                        }
                                    }
                                    break;
                                case REMAINDER:
                                    break;
                                default:
                                    ;
                            }
                        out << "):(s'=" << t.successor << ")";
                    default:
                        ;
                }

            }
            out << ";\n";
        }
    }
    out << "\nendmodule \n \n";
    out << "label \"complete\"=(s=" << success << ");\n";
}

MDP::statet_a MDP::get_init_state() {
    bool found = false;
    statet_a result;
    for (const auto& s : states) {
        if (s.init == true && found == false) {
            found = true;
            result = s;
        } else if (s.init == true && found == true) {
            std::cout << "error in get_init_state: found 2 initial states \n";
            throw std::exception();
        }
    }
    //if we get here throw exception
    if (found == false) {
        std::cout << "error in get_init_state: no initial state found \n";
        throw std::exception();
    }
    return result;
}