#ifndef pctl_tokenizer_h
#define pctl_tokenizer_h
#include <string>
#include <vector>
enum tokenkindt {PROB, UNTIL, IDENTIFIER, X, NOT, AND, OR, EQ, SP, 
	LT, GT, LP, RP, SLP, SRP, FI, GL, MIN, MAX, QUE, LE, GE, IMPLIES, BUNTIL};
struct tokent {tokenkindt kind;std::string label;};
std::vector<tokent> pctl_tokenizer(std::string);
void token(tokent token);
#endif
