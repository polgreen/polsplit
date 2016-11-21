#include <exception>
#include <iostream>


struct pctlsyntax :std::exception {
	const char* what() const noexcept {return "PCTL syntax error \n";}
};




struct ooops : std::exception {
  const char* what() const noexcept {return "Ooops!\n";}
};