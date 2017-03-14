
echo "Bash version ${BASH_VERSION}..."
export LD_LIBRARY_PATH=$LD_LIBRARY_PATh:/users/elipol/gsl/lib
export PATH=$PATH:/users/elipol/prism-svn/prism/bin
for i in `seq 15 5 75`;
do
   >model_param.h
	echo "#ifndef MODELPARAM_H">>model_param.h
	echo "#define MODELPARAM_H">>model_param.h
	echo "#define PARAM1 " $i >>model_param.h

	echo "#define PARAM2 " $i >>model_param.h
	echo "#endif">>model_param.h
	./DKRcompile
	
	for trace_length in `seq 10 10 100`;
	do
		for number_traces in `seq 10 10 100`;
		do
			./baever --MDP 3 --traces $number_traces --tracelength $trace_length --batch 10 --explicit_strategy
			./baever --MDP 3 --traces $number_traces --tracelength $trace_length --batch 10 --random_strategy
			./baever --MDP 3 --traces $number_traces --tracelength $trace_length --batch 10 --first_strategy	
		done
	done				
done   

