#ifndef propcheck_h
#define propcheck_h
#include "markov_chain.h"
satset comparesets(satset set1, satset set2);
void output_satset(satset s);
satset checkNOT(satset set1);
void set_all_states();
#endif