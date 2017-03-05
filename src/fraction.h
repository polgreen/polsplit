#ifndef fraction_h
#define fraction_h
#include <iostream>

struct fractiont{
	fractiont() { nom=0; denom=0; }
	fractiont(signed s) { nom=s; denom=1; }
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
	friend fractiont operator+ (fractiont, fractiont);
	void simplify();
	void zero();
	void one();
	friend std::ostream& operator<<(std::ostream& out, fractiont f)
	{f.output(out); 
		return out;}
	void output(std::ostream &out);
};
#endif				
