#ifndef fraction_h
#define fraction_h
#include <iostream>

struct fractiont {
    fractiont();

    fractiont(signed s) {
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
    friend fractiont operator+(fractiont, fractiont);
    void simplify();
    void zero();
    void one();
    void avg(unsigned);

    friend std::ostream& operator<<(std::ostream& out, fractiont f) {
        f.output(out);
        return out;
    }
    void output(std::ostream &out);
};
#endif    