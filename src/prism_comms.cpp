#include <vector>
#include <string>
#include <random>
#include <iostream>
#include <cassert>
#include <sstream>
#include <fstream>
#include <cstdlib>

#include "model.h"
#include "fraction.h"

std::string ssystem(const char *command) {
    char tmpname [L_tmpnam];
    std::tmpnam(tmpname);
    std::string scommand = command;
    std::string cmd = scommand + " >> " + tmpname;
    std::system(cmd.c_str());
    std::ifstream file(tmpname, std::ios::in);
    std::string result;
    if (file) {
        while (!file.eof()) result.push_back(file.get());
        file.close();
    }
    remove(tmpname);
    return result;
}

std::ofstream getPRISMFile() {
    std::ofstream prismfile("prismfile.pm");
    if (!prismfile) {
        throw std::exception();
    }

    return prismfile;
}

std::string prepCmd() {
    std::string command("prism prismfile.pm ../../../propertyfile.props -param ");
    return command;
}

void MC::callPrism() {

    std::ofstream prismfile = getPRISMFile();
    this->outputPRISM(prismfile);
    prismfile.close();
    std::string command = prepCmd();
    std::string result;
    char letter = 'a';
    for (unsigned p_index = 1; p_index < modelparams.size(); p_index++) {
        command += static_cast<char> (letter + p_index - 1);
        command += "=0:1,";
    }
    result = ssystem(command.c_str());
    for (const auto s : result) {
        std::cout << s;
    }
    if (verbose > 1)
        std::cout << "Parsing prism output to find feasible set \n";
    prism_find(result);
}

void MC::outputPRISM(std::ostream &out) {
    char letter = 'a';
    out << "dtmc \n\n";

    for (unsigned p_index = 1; p_index < modelparams.size(); p_index++) {
        out << "const double ";
        out << static_cast<char> (letter + p_index - 1) << ";\n";
    }
    out << "\n\nmodule test \n";
    out << "\n // local state \n s: [0.."
            << states.size() - 1 << "] init " << get_init_state().ID;
    out << ";\n \n";

    for (auto & s : states) {
        out << "[] s=" << s.ID << " -> ";
        bool first_t = true;
        for (auto & t : s.transitions) {
            if (first_t) {
                first_t = false;
            } else {
                out << " + ";
            }
            bool first_p = true;
            switch (t.type) {
                case NEWCONST:
                case CONST: t.prob.output(out);
                    out << " : (s'=" << t.successor << ") ";
                    break;
                case NEWFUNCTION:
                case FUNCTION:
                    for (auto & p : t.params) {
                        if (first_p) {
                            first_p = false;
                        } else {
                            out << " + ";
                        }
                        p.first.output(out);
                        out << "*";
                        if (p.second > 0) {
                            out << static_cast<char> (letter + p.second - 1);
                        }
                    }
                    out << " : (s'=" << t.successor << ")";
                    break;
                case NEWREMAINDER:
                case REMAINDER: out << "(1";
                    for (auto & t2 : s.transitions)
                        switch (t2.type) {
                            case NEWCONST:
                            case CONST: out << "-";
                                t2.prob.output(out);
                                break;
                            case NEWFUNCTION:
                            case FUNCTION:
                                for (auto & p2 : t2.params) {
                                    out << "-";
                                    p2.first.output(out);
                                    out << "*";
                                    if (p2.second > 0) {
                                        out << static_cast<char> (letter + p2.second - 1);
                                    }
                                }
                                break;
                            case NEWREMAINDER:
                            case REMAINDER: break;
                            default:;
                        }
                    out << "):(s'=" << t.successor << ")";
                default:;
            }

        }
        out << ";\n";
    }
    out << "\nendmodule \n \n";
    out << "label \"complete\"=(s=" << success << ");\n";

}



