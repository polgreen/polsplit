# polsplit


model.cpp 

MC::check() - checks whether Markov chain violates any of:
- must have states
- sum of all transition probabilities on any state must be <=1
- sum of all parameter multipliers on any transition must be <=1
- must be no negative transition probabilities

MC::get_parameterised_states() - finds transitions with parameters and returns a vector of pairs. The first element is the state, the second element is the transition number

MC::weighting(transition, state) - given a transition and the state it came from, returns the probability of that transition calculated using the current model parameter values

MC::remainderWeight(s) - calculates the probability of the remainder transition, i.e., 1 - all other transitions

MC::get_init_state() - returns initial state

void printstate(state) - prints to cout

MC::outputMC(std::ostream) - prints markov chain

models.cpp - contains functions that return hard-coded models


pctl_parser.cpp - contains functions to parse a pctl property
pctl_tokenizer.cpp - contains functions to tokenize an inputted pctl property

fraction.cpp - fraction functions

markov_chain.cpp = functions to get data from markov chain

propcheck.cpp = checks pctl property is valid

prism_comms.cpp - contains functions to write a prism file, and send file to prism parameter synthesis tool




