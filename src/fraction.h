/*****************************\
          (\.---./)
 /.-.-.\
          /| 0_0 |\
         |_`-(v)-'_|
 \`-._._.-'/      .-.
 -~-(((.`-\_|_/-'.)))-~' <_
 `.     .'
 `._.'
 BaeVer
 Bayesian Verification for DTMCs
 -----~--~---~~~----~-`.-;~
 elizabeth.polgreen@cs.ox.ac.uk
 \********************************/
#ifndef fraction_h
#define fraction_h
#include <iostream>
#include <vector>

struct fractiont
{
    fractiont()
    {
      nom = 0;
      denom = 0;
    }
    fractiont(signed s)
    {
      nom = s;
      denom = 1;
    }
    int nom;
    int denom;
    friend fractiont operator+(fractiont, fractiont);
    friend bool operator==(fractiont, fractiont);
    friend bool operator!=(fractiont, fractiont);
    friend bool operator>=(fractiont, fractiont);
    friend bool operator<=(fractiont, fractiont);
    friend bool operator>(fractiont, fractiont);
    friend bool operator<(fractiont, fractiont);
    friend fractiont operator*(fractiont, fractiont);
    friend fractiont operator/(fractiont, fractiont);
    friend fractiont operator-(fractiont, fractiont);
    friend fractiont operator-(int, fractiont);
    friend fractiont operator+(fractiont, fractiont);
    void simplify();
    void zero();
    void one();
    friend std::ostream& operator<<(std::ostream& out, fractiont f)
    {
      f.output(out);
      return out;
    }
    void output(std::ostream &out);
};

fractiont double_to_fraction(double d);
fractiont frac_abs(fractiont);
double fraction_to_double(fractiont);
std::vector<fractiont> intvector_to_fractions(const std::vector<int>&);
std::vector<fractiont> intvector_to_fractions(const std::vector<unsigned>&);
#endif				

