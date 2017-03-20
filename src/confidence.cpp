#include <vector>
#include <random>
#include <time.h>
#include <random>
#include <iostream>
#include <cassert>

#include "fraction.h"
#include "distributions.h"
#include "model.h"

void MC::reset_confidence() {
    if (verbose > 1)
        std::cout << "reset confidence\n";
    overall_confidence.zero();
}

void MC::sample_params_update_conf() {
    if (verbose > 1)
        std::cout << "\nsample params and update confidence \n";
    int cntr = 0;
    while (cntr < std::numeric_limits<int>::max()) {
        std::vector<double> sample;
        std::vector<fractiont> temp_param_confidence = param_confidence;
        for (unsigned i = 1; i < modelparams.size(); i++) {
            bool in_possible_set = false;
            double s;
            while (!in_possible_set) {
                s = rd.beta(parametercounts[i] + beta_prior_param1[i], inv_parametercounts[i] + beta_prior_param2[i]);
                if (s <= param_upper_bounds[i] & s > param_lower_bounds[i])
                    in_possible_set = true;
            }
            sample.push_back(s);

        }
        switch (is_in_range(sample)) {
            case 0: overall_confidence.denom++;
                cntr = std::numeric_limits<int>::max();
                break;
            case 1: overall_confidence.nom++;
                overall_confidence.denom++;
                cntr = std::numeric_limits<int>::max();
                break;
            case 2: param_confidence = temp_param_confidence;
                cntr++;
                break;
            default: throw std::exception();
                break;
        }
    }
    if (verbose > 1)
        std::cout << "confidence = " << overall_confidence.nom << "/" << overall_confidence.denom << "\n";
}

//fractiont computeconfidence(std::vector<fractiont> conf_vector) {
//    fractiont result = 1;
//    for (const auto c : conf_vector) {
//        result = result * c;
//    }
//
//    return result;
//}

void MC::confidencecalc() {
    if (verbose > 1)
        std::cout << "confidence calc \n";
    // initRndDistribution();
    std::vector< std::pair < statet, unsigned> > param_states;
    reset_confidence();
    param_states = get_parameterised_states();
    if (param_states.size() == 0)
        return;
    std::vector<int>total_paramcounts(modelparams.size());
    std::vector<int>total_inv_paramcounts(modelparams.size());
    for (unsigned i = 0; i < int_samples; i++) {
        if (need_state_splitting) {
            if (verbose > 1)
                std::cout << "State splitting \n";
            get_random_model_params();
            sample_D_star(param_states);
            for (int i = 0; i < modelparams.size(); i++) {
                total_paramcounts[i] += parametercounts[i];
                total_inv_paramcounts[i] += inv_parametercounts[i];
            }
        }
        sample_params_update_conf();
    }
    for (int i = 1; i < modelparams.size(); i++) {
        parametercounts[i] = total_paramcounts[i] / int_samples;
        inv_parametercounts[i] = total_inv_paramcounts[i] / int_samples;
    }
    updatePriors();

}

void MC::updatePriors() {
    for (int i = 1; i < modelparams.size(); i++) {
        beta_prior_param1[i] += parametercounts[i];
        beta_prior_param2[i] += inv_parametercounts[i];
        if (beta_prior_param1[i] == 0) {
            beta_prior_param1[i] = 1;
        }
        if (beta_prior_param2[i] == 0) {
            beta_prior_param2[i] = 1;
        }
        if (verbose > 0)
            std::cout << "updated prior for p" << i << " " << beta_prior_param1[i] << " " << beta_prior_param2[i] << std::endl;
    }
}
