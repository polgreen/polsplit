
#include "fraction.h"
#include <iostream>

unsigned gcdr(unsigned a, unsigned b) {
    if (a == 0) return b;
    return gcdr(b % a, a);
}

void fractiont::simplify() {
    if (nom == 0 || denom == 0) {//do nothing
    } else {
        unsigned GCD;
        GCD = gcdr(nom, denom);
        nom = nom / GCD;
        denom = denom / GCD;
    }
}

void fractiont::output(std::ostream &out) {
    simplify();
    if (nom == 0) {
        out << 0;
    } else if (nom == denom) {
        out << 1;
    } else {
        out << nom << "/" << denom;
    }
}

fractiont operator+(fractiont f1, fractiont f2) {
    fractiont result;
    if (f1.nom == 0 || f1.denom == 0) {
        result.nom = f2.nom;
        result.denom = f2.denom;
    } else if (f2.nom == 0 || f2.denom == 0) {
        result.nom = f1.nom;
        result.denom = f1.denom;
    } else {
        result.denom = f2.denom * f1.denom;
        result.nom = f1.denom * f2.nom + f1.nom * f2.denom;
    }
    result.simplify();
    return result;
}

fractiont operator*(fractiont f1, fractiont f2) {
    fractiont result;
    result.denom = f1.denom * f2.denom;
    result.nom = f1.nom * f2.nom;
    result.simplify();
    return result;
}

fractiont operator/(fractiont f1, fractiont f2) {
    fractiont result;
    result.denom = f1.denom * f2.nom;
    result.nom = f2.denom * f1.nom;
    result.simplify();
    return result;
}

fractiont operator-(fractiont f1, fractiont f2) {
    fractiont result;
    if (f2.nom == 0) {
        result.nom = f1.nom;
        result.denom = f1.denom;
    } else if (f1.nom == 0) {
        result.nom = -f2.nom;
        result.denom = -f2.denom;
    } else {
        result.denom = f1.denom * f2.denom;
        result.nom = f2.denom * f1.nom - f2.nom * f1.denom;
    }
    //	result.simplify();
    return result;
}

bool operator==(fractiont f1, fractiont f2) {
    return (f1.nom * f2.denom == f1.denom * f2.nom);
}

bool operator!=(fractiont f1, fractiont f2) {
    return (f1.nom * f2.denom != f1.denom * f2.nom);
}

bool operator>(fractiont f1, fractiont f2) {
    return ((f1.nom * f2.denom) > (f1.denom * f2.nom));
}

bool operator<(fractiont f1, fractiont f2) {
    return ((f1.nom * f2.denom) < (f1.denom * f2.nom));
}

bool operator<=(fractiont f1, fractiont f2) {
    return ((f1.nom * f2.denom) <= (f1.denom * f2.nom));
}

bool operator>=(fractiont f1, fractiont f2) {
    return ((f1.nom * f2.denom) >= (f1.denom * f2.nom));
}

void fractiont::zero() {
    nom = 0;
    denom = 0;
}

void fractiont::one() {
    nom = 1;
    denom = 1;
}
