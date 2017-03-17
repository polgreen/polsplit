/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <vector>
#include <random>
#include <time.h>
#include <random>
#include <iostream>
#include <cassert>

#include "fraction.h"
#include "distributions.h"
#include "model.h"
#include "model_mdp.h"

void MDP::confidencecalc() {
    if (verbose > 1)
        std::cout << "confidence calc \n";
    //initRndDistribution();
    std::vector< std::tuple <statet_a, unsigned, unsigned> > param_states;
    reset_confidence();
    param_states = get_parameterised_states();
    if (param_states.size() == 0)
        return;
    param_states = get_parameterised_states();
    std::vector<int>total_paramcounts(modelparams.size());
    std::vector<int>total_inv_paramcounts(modelparams.size());
    for (unsigned i = 0; i < int_samples; i++) {
        if (need_state_splitting) {
            if (verbose > 1)
                std::cout << "State splitting \n";
            get_random_model_params();
            sample_D_star(param_states);
            for (int j = 0; j < modelparams.size(); j++) {
                total_paramcounts[j] += parametercounts[j];
                total_inv_paramcounts[j] += inv_parametercounts[j];
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

std::vector< std::tuple <MDP::statet_a, unsigned, unsigned> > MDP::get_parameterised_states() {
    if (states.size() == 0) {
        std::cout << "no model states found\n";
        throw std::exception();
    }
    std::vector< std::tuple < statet_a, unsigned, unsigned> > result;
    std::tuple<statet_a, unsigned, unsigned> tuple;

    for (auto &s : states) {
        for (unsigned a = 0; a < s.actions.size(); a++) {
            for (unsigned t = 0; t < s.actions[a].first.size(); t++) {
                if ((s.actions[a].first[t].type == FUNCTION || s.actions[a].first[t].type == NEWFUNCTION)) {
                    if (verbose > 1)
                        std::cout << "Found param state " << s.ID << " , param transition to " << s.actions[a].first[t].successor << " , under the action " << a << "\n";
                    tuple = std::make_tuple(s, a, t);
                    result.push_back(tuple);
                }
            }
        }
    }
    return result;
}

void MDP::sample_D_star(std::vector< std::tuple <statet_a, unsigned, unsigned> > &param_states) {
    need_state_splitting = false;
    if (verbose > 1)
        std::cout << "Sample D* \n";
    std::vector<unsigned> param_counts;
    std::vector<unsigned> inv_param_counts;
    param_counts.resize(parametercounts.size());
    inv_param_counts.resize(inv_parametercounts.size());
    unsigned kcount = 0;
    double prob;
    bool all_constants_equal_one = true;
    for (auto ps : param_states) {
        statet_a s = std::get<0>(ps);
        unsigned action_number = std::get<1>(ps);
        unsigned transition_number = std::get<2>(ps);
        if (verbose > 1)
            std::cout << "Param state S" << s.ID << ":\n";
        for (const auto t : s.actions[action_number].first) {
            if (t.type == REMAINDER)
                break;
            if (verbose > 1)
                std::cout << " transition to S" << t.successor << " :";
            if (t.params.size() == 1) {
                if (t.params[0].first == 1) {
                    param_counts[t.params[0].second] = param_counts[t.params[0].second] + t.count;
                    inv_param_counts[t.params[0].second] = inv_param_counts[t.params[0].second] + s.sum_outputs(action_number) - t.count;
                    if (verbose > 1)
                        std::cout << " only 1 parameter, P" << t.params[0].second << ": count + "
                            << t.count << " = " << param_counts[t.params[0].second] << ", inverse count + " << s.sum_outputs(action_number) - t.count
                            << "= " << inv_param_counts[t.params[0].second] << "\n";

                } else {
                    need_state_splitting = true;
                    if (verbose > 3)
                        std::cout << " parameter has a multiplier";
                    param_counts[t.params[0].second] = param_counts[t.params[0].second] + t.count;
                    prob = t.params[0].first.nom / (double) t.params[0].first.denom;
                    while (kcount < t.count) {
                        kcount = rd.binomial(prob, s.actions[action_number].second);
                        std::cout << "Sampled kcount " << kcount << std::flush;
                    }
                    inv_param_counts[t.params[0].second] = inv_param_counts[t.params[0].second] + kcount - t.count;
                    if (verbose > 1)
                        std::cout << " only 1 parameter," << t.params[0].first.nom << "/" << t.params[0].first.denom << "*P" << t.params[0].second
                            << ": count + "
                            << t.count << " = " << param_counts[t.params[0].second] << ", inverse count + " << kcount - t.count
                            << "= " << inv_param_counts[t.params[0].second] << "\n";
                }
            } else {
                need_state_splitting = true;
                for (const auto p : t.params) {
                    if (p.first != 1) {
                        all_constants_equal_one = false;
                    }
                }
                if (all_constants_equal_one) {
                    std::vector<double> probs(t.params.size());
                    std::vector<unsigned> sample(t.params.size());

                    for (unsigned i = 0; i < t.params.size(); i++) {
                        probs[i] = modelparams[t.params[i].second].nom / (double) modelparams[t.params[i].second].denom;
                    }
                    rd.multinomial(t.params.size(), s.actions[action_number].second, probs, sample);

                    for (unsigned i = 0; i < t.params.size(); i++) {
                        param_counts[i] = param_counts[i] + sample[i];
                        inv_param_counts[i] = param_counts[i] + s.actions[action_number].second - sample[i];
                    }

                } else {
                    std::vector<double> probs1(t.params.size() + 1);
                    std::vector<unsigned> sample1(t.params.size() + 1);
                    std::vector<double> probs2(t.params.size());
                    std::vector<unsigned> sample2(t.params.size());
                    probs1[t.params.size() + 1] = 1;
                    for (unsigned i = 0; i < t.params.size(); i++) {
                        probs2[i] = modelparams[t.params[i].second].nom / (double) modelparams[t.params[i].second].denom;
                        probs1[i] = 1 - probs2[i];
                        probs1[t.params.size() + 1] = probs1[t.params.size() + 1] - t.params[i].first.nom / (double) t.params[i].first.denom;
                    }

                    rd.multinomial(t.params.size(), t.count, probs2, sample2);
                    rd.multinomial(t.params.size() + 1, s.actions[action_number].second - t.count, probs1, sample1);

                    for (unsigned i = 0; i < t.params.size(); i++) {
                        param_counts[i] = param_counts[i] + sample2[i];
                        inv_param_counts[i] = param_counts[i] + sample1[i];
                    }
                }
            }
        }
    }

    parametercounts = param_counts;
    inv_parametercounts = inv_param_counts;
    if (verbose > 1) {
        std::cout << "\nParam counts: ";
        for (int i = 0; i < param_counts.size(); i++) {
            std::cout << "P" << i << ": " << param_counts[i] << "/" << inv_param_counts[i] << " ";
        }
        std::cout << "\n";
    }

}

unsigned MDP::statet_a::sum_outputs(unsigned action_number) {
    unsigned sum = 0;
    for (const auto &t : actions[action_number].first) {
        sum = sum + t.count;
    }
    return sum;
}
