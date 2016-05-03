
#include "fraction.h"
#include <iostream>

void fractiont::output(std::ostream &out)
{
	if(nom==0){out<<0;}
	else if(nom==denom){out<<1;}
	else{out<<nom<<"/"<<denom;}
}


fractiont fractiont::add(fractiont frac)
{
	fractiont result;
	if(frac.nom==0||frac.denom==0)
		{result.nom=nom; result.denom=denom;}
	if(nom==0||denom==0)
		{result.nom=frac.nom; result.denom=frac.denom;}
	else{
		result.denom = denom * frac.denom;
	result.nom = frac.denom*nom + frac.nom*denom;}
	return result;
}

fractiont fractiont::multiply(fractiont frac)
{
	fractiont result;
	result.denom = denom * frac.denom;
	result.nom = nom* frac.nom;
	return result;
}

fractiont fractiont::divide(fractiont frac)
{
	fractiont result;
	result.denom = denom * frac.nom;
	result.nom = frac.denom * nom;
	return result;
}



fractiont fractiont::subtract(fractiont frac)
{
	fractiont result;
	if(frac.nom==0||frac.denom==0)
		{result.nom=nom; result.denom=denom;}
	if(nom==0||denom==0)
		{result.nom=-frac.nom; result.denom=-frac.denom;}

	result.denom = denom * frac.denom;
	result.nom = frac.denom*nom - frac.nom*denom;
	return result;	
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