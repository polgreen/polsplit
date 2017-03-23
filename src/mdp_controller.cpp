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
    } else if (std::string(argv[i]) == "--trace_confidence") {
        trace_confidence = true;
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
        std::cout << "Randomized finite memory strategy\n";

    } else if (strategy_type_cap == 4) {
        std::cout << "Strategy: no_strategy\n";
    }

    if (trace_confidence)
        std::cout << "Calculating confidence after each trace\n";


}

void MDP_cmdvars::init_process(int verbose, int number_of_traces, int trace_length, long int_samples, long batch) {
    std::ofstream results;
    std::string resultfilename = ("Results.csv");
    results.open(resultfilename, std::ofstream::out | std::ofstream::app);

    for (unsigned a = 15; a <= 75; a += 5) {
        rud_param_values.resize(2);
        rud_param_values[0] = a;
        rud_param_values[1] = a;
        std::cout << "\n\nRudimentary Param Value: " << a << "\n\n";
        MDP model = get_MDP_instance(1);
        model.verbose = verbose;
        model.number_of_traces = number_of_traces;
        model.trace_length = trace_length;
        model.int_samples = int_samples;
        model.strategy_type = strategy_type_cap;
        model.initRndDistribution();
        for (long i = 0; i < batch; i++) {
            model.prepModel();
            if (i == 0) {
                model.callPrism();
            }
            for (unsigned n = 0; n < model.number_of_traces; n++) {
                model.synthStrategy();
                if (model.finiteMemMode == 0) {
                    MC inducd_model = induceMarkovChain(model);
                    inducd_model.get_data();
                    inducd_model.confidencecalc();
                    model.overall_confidence = inducd_model.overall_confidence;
                    model.beta_prior_param1 = inducd_model.beta_prior_param1;
                    model.beta_prior_param2 = inducd_model.beta_prior_param2;
                } else {
                    MDP exp_model = model;
                    exp_model.get_data();
                    exp_model.confidencecalc();
                    model.overall_confidence = exp_model.overall_confidence;
                    model.beta_prior_param1 = exp_model.beta_prior_param1;
                    model.beta_prior_param2 = exp_model.beta_prior_param2;
                    if (trace_confidence)
                        model.displayConfidence(results, n + 1, i + 1, a);
                }
            }
            if (!trace_confidence)
                model.displayConfidence(results);
            //std::cout << " p1 confidence " << model.param_confidence[0] << "/   p2 confidence " << model.param_confidence[1] << "\n";
        }
    }
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

statet MDP::statet_a::getMCStateStruc(int actionNumber) {
    statet s;
    s.ID = ID;
    s.init = init;
    s.transitions = actions[actionNumber].first;
    return s;


}

void MDP::displayConfidence(std::ofstream& results) {
    results << "MDP , " << model_num << " , ";
    for (const auto &p : modelparams)
        results << fraction_to_double(p) << " , ";
    results << number_of_traces << " , " << trace_length << " , " << int_samples << " , " << strategy_type << " , ";
    results << fraction_to_double(overall_confidence) << " , ";
    for (int i = 1; i < modelparams.size(); i++) {
        results << beta_prior_param1[i] << " , " << beta_prior_param2[i] << " , ";
    }
    results << std::endl;
    std::cout << "overall confidence: " << overall_confidence.nom << "/" << overall_confidence.denom << "\n";

}

void MDP::displayConfidence(std::ofstream& results, unsigned trace_number, unsigned batch_number, unsigned param_increment) {
    results << "MDP , " << model_num << " , ";
    for (const auto &p : modelparams)
        results << fraction_to_double(p) << " , ";
    results << number_of_traces << " , " << trace_length << " , " << int_samples << " , " << strategy_type << " , " << trace_number << " , " << batch_number << " , " << param_increment << " , ";
    results << fraction_to_double(overall_confidence) << " , ";
    for (int i = 1; i < modelparams.size(); i++) {
        results << beta_prior_param1[i] << " , " << beta_prior_param2[i] << " , ";
    }
    results << std::endl;
    //std::cout << "overall confidence: " << overall_confidence.nom << "/" << overall_confidence.denom << "\n";

}