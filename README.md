# BaeVer

Data efficient Bayesian verification for DTMCs: 
http://link.springer.com/chapter/10.1007%2F978-3-319-43425-4_3

This is not the experiment implementation used in the paper (which was done in Matlab), but follows the same methodology. 

To compile the code:
- git clone https://github.com/polgreen/BaeVer
- download and install Gnu Scientific Library https://www.gnu.org/software/gsl/, instructions: http://coral.ise.lehigh.edu/jild13/2016/07/11/hello/
- make sure GSL is located here: /usr/local/include/gsl, if it's not, you will need to change the BaeVer compile file accordingly
- download PRISM and add the path to your profile (or put the binary in BaeVer/src) http://www.prismmodelchecker.org/download.php
- cd BaeVer/src
- ./compile

The model and system in Baever is hard coded in models.cpp, the property is in propertyfile.props. By default, it runs with 10 traces of length 10 transitions, but this can be changed with the command line arguments
- --tracelength N
- --traces N

e.g.,, to run 10 traces of length 100: ./baever --tracelength 100 --traces 10

The system runs with greater verbosity with --debug


