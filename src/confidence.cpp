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
    confidence.resize(modelparams.size());
    for (auto c : confidence) {
        c.nom = 0;
        c.denom = 0;
    }
}

void MC::sample_params_update_conf(random_distribution &rd) {
    if (verbose > 1)
        std::cout << "\nsample params and update confidence \n";
    std::vector<double> sample;
    for (unsigned i = 1; i < modelparams.size(); i++) {
        sample.push_back(rd.beta(parametercounts[i], inv_parametercounts[i]));
    }
    if (is_in_range(sample)) {
        overall_confidence.nom++;
    }
    overall_confidence.denom++;
    if (verbose > 1)
        std::cout << "confidence = " << overall_confidence.nom << "/" << overall_confidence.denom << "\n";
}

fractiont computeconfidence(std::vector<fractiont> conf_vector) {
    fractiont result = 1;
    for (const auto c : conf_vector) {
        result = result * c;
    }

    return result;
}

void MC::confidencecalc() {
    if (verbose > 1)
        std::cout << "confidence calc \n";
    unsigned seed = 0;
    random_distribution rd;
    if (verbose > 1)
        std::cout << " Using seed of " << seed << "\n";
    rd.set_seed(seed); //we use the same seed so that we can reproduce experiments
    std::vector<fractiont> result(modelparams.size(), 0);
    std::vector< std::pair < statet, unsigned> > param_states;
    reset_confidence();

    param_states = get_parameterised_states();

    for (unsigned i = 0; i < int_samples; i++) {
        get_random_model_params(rd);
        sample_D_star(param_states, rd);
        sample_params_update_conf(rd);

    }
    std::cout << "overall confidence " << overall_confidence.nom << "/" << overall_confidence.denom << "\n";
    ;

}

