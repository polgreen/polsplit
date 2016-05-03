#ifndef fraction_h
#define fraction_h
#include <iostream>

struct fractiont{int nom; 
				int denom;
				fractiont add(fractiont frac);
				fractiont multiply(fractiont frac);
				fractiont divide(fractiont frac);
				fractiont subtract(fractiont frac);
				void zero();
				void one();
				void output(std::ostream &out);

				};
#endif				