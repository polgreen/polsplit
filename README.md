# BaeVer

Data efficient Bayesian verification for DTMCs: 
http://link.springer.com/chapter/10.1007%2F978-3-319-43425-4_3

This is not the experiment implementation used in the paper (which was done in Matlab), but follows the same methodology. 

This implementation takes a PCTL property as input, e.g.,:
./a.out "P>=0.5 [F "a"]"

The DTMC used by the implementation is inputted via models.cpp (i.e., the code must be changed). 

It parses the PCTL property (pctl_parser.cpp, pctl_tokenizer.cpp), and sends the model and property to PRISM for synthesis (prism_comms.cpp). 

TODO: here there needs to be a parser that reads the output of prism's synthesis

trace.cpp generates sample traces through the model.

distributions.cpp contains the confidence calc functions, which calculate the confidence the model satisfies the property (TODO: the upper and lower bounds from the PRIS parameter synthesis need to be input here). Note that we don't do the *actual* state splitting as in the paper, instead we split the data as if we had split the states, i.e., we skip the intermediate steps and go straight from data to output result.

This repository is a mess.. the key thing missing at the moment is the parser for the output from PRISM, so that the synthesis result can be inputted into the confidence calc.


Other stuff in this repo:
- initial attempts at actually splitting the states
- the beginnings of a statistical model checker (see polgreen/polcheck for more)


******************************
TO DO LIST
****************************
- PRISM output PARSER!!!! 
