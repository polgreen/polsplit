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

#include "fraction.h"
#include <iostream>

#define FRAC_PRECISION 1000

unsigned gcdr(unsigned a, unsigned b)
{
  if (a == 0)
    return b;
  return gcdr(b % a, a);
}

void fractiont::simplify()
{
  if (nom == 0 || denom == 0)
  { //do nothing
  }
  else
  {
    unsigned GCD;
    GCD = gcdr(nom, denom);
    nom = nom / GCD;
    denom = denom / GCD;
  }
}

void fractiont::output(std::ostream &out)
{
  simplify();
  if (nom == 0)
  {
    out << 0;
  }
  else if (nom == denom)
  {
    out << 1;
  }
  else
  {
    out << nom << "/" << denom;
  }
}

fractiont operator+(fractiont f1, fractiont f2)
{
  fractiont result;
  if (f1.nom == 0 || f1.denom == 0)
  {
    result.nom = f2.nom;
    result.denom = f2.denom;
  }
  else if (f2.nom == 0 || f2.denom == 0)
  {
    result.nom = f1.nom;
    result.denom = f1.denom;
  }
  else
  {
    result.denom = f2.denom * f1.denom;
    result.nom = f1.denom * f2.nom + f1.nom * f2.denom;
  }
  result.simplify();
  return result;
}

fractiont frac_abs(fractiont f1)
{
  if (f1 < 0)
    f1.nom = -f1.nom;
  return f1;
}

fractiont operator*(fractiont f1, fractiont f2)
{
  fractiont result;
  result.denom = f1.denom * f2.denom;
  result.nom = f1.nom * f2.nom;
  //std::cout<<std::endl<<f1.nom<<"/"<<f1.denom<<"*"<<f2.nom<<"/"<<f2.denom<<"="<<result.nom<<"/"<<result.denom<<std::endl;
  result.simplify();
  return result;
}

fractiont operator/(fractiont f1, fractiont f2)
{
  fractiont result;
  result.denom = f1.denom * f2.nom;
  result.nom = f2.denom * f1.nom;
  result.simplify();
  return result;
}

fractiont operator-(fractiont f1, fractiont f2)
{
  fractiont result;
  if (f2.nom == 0)
  {
    result.nom = f1.nom;
    result.denom = f1.denom;
  }
  else if (f1.nom == 0)
  {
    result.nom = -f2.nom;
    result.denom = -f2.denom;
  }
  else
  {
    result.denom = f1.denom * f2.denom;
    result.nom = f2.denom * f1.nom - f2.nom * f1.denom;
  }
//	result.simplify();
  return result;
}

fractiont operator-(int i1, fractiont f2)
{
  fractiont result;
  fractiont f1;
  f1.nom = i1;
  f1.denom = 1;
  if (f2.nom == 0)
  {
    result.nom = f1.nom;
    result.denom = f1.denom;
  }
  else if (f1.nom == 0)
  {
    result.nom = -f2.nom;
    result.denom = -f2.denom;
  }
  else
  {
    result.denom = f1.denom * f2.denom;
    result.nom = f2.denom * f1.nom - f2.nom * f1.denom;
  }
//  result.simplify();
  return result;
}

bool operator==(fractiont f1, fractiont f2)
{
  return (f1.nom * f2.denom == f1.denom * f2.nom);
}

bool operator!=(fractiont f1, fractiont f2)
{
  return (f1.nom * f2.denom != f1.denom * f2.nom);
}

bool operator>(fractiont f1, fractiont f2)
{
  return ((f1.nom * f2.denom) > (f1.denom * f2.nom));
}

bool operator<(fractiont f1, fractiont f2)
{
  return ((f1.nom * f2.denom) < (f1.denom * f2.nom));
}

bool operator<=(fractiont f1, fractiont f2)
{
  return ((f1.nom * f2.denom) <= (f1.denom * f2.nom));
}

bool operator>=(fractiont f1, fractiont f2)
{
  return ((f1.nom * f2.denom) >= (f1.denom * f2.nom));
}

void fractiont::zero()
{
  nom = 0;
  denom = 0;
}

void fractiont::one()
{
  nom = 1;
  denom = 1;
}

fractiont double_to_fraction(double d)
{
  fractiont f1;
  f1.nom = d * FRAC_PRECISION;
  f1.denom = FRAC_PRECISION;
  f1.simplify();
  return f1;
}

double fraction_to_double(fractiont f1)
{
  if(f1.denom==0)
    return 0;

  return (double)f1.nom/(double)f1.denom;
}

std::vector<fractiont> intvector_to_fractions(const std::vector<int>& v1)
{
  std::vector<fractiont> result(v1.size());
  for(int i=0; i<v1.size(); i++)
  {
    result[i].nom=v1[i];
    result[i].denom=1;
  }
  return result;
}

std::vector<fractiont> intvector_to_fractions(const std::vector<unsigned>& v1)
{
  std::vector<fractiont> result(v1.size());
  for(int i=0; i<v1.size(); i++)
  {
    result[i].nom=v1[i];
    result[i].denom=1;
  }
  return result;
}









