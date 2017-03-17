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
    std::vector<double> sample;
    for (unsigned i = 1; i < modelparams.size(); i++) {
        sample.push_back(rd.beta(parametercounts[i] + beta_prior_param1[i], inv_parametercounts[i] + beta_prior_param2[i]));
    }
    if (is_in_range(sample)) {
        overall_confidence.nom++;
    }
    overall_confidence.denom++;
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
        get_random_model_params();
        sample_D_star(param_states);
        sample_params_update_conf();
        for (int i = 0; i < modelparams.size(); i++) {
            total_paramcounts[i] += parametercounts[i];
            total_inv_paramcounts[i] += inv_parametercounts[i];
        }
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
        if (verbose > 0)
            std::cout << "updated prior for p" << i << " " << beta_prior_param1[i] << " " << beta_prior_param2[i] << std::endl;
    }
}