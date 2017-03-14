
echo "Bash version ${BASH_VERSION}..."

for i in `seq 15 5 75`;
do
   >model_param.h
	echo "#ifndef MODELPARAM_H">>model_param.h
	echo "#define MODELPARAM_H">>model_param.h
	echo "#define PARAM1 " $i >>model_param.h

	echo "#define PARAM2 " $i >>model_param.h
	echo "#endif">>model_param.h
	./compile
	
	for trace_length in `seq 10 30 100`;
	do
		for number_traces in `seq 10 100 210`;
		do
			./baever --MDP 3 --traces $number_traces --tracelength $trace_length --batch 10 --explicit_strategy
			./baever --MDP 3 --traces $number_traces --tracelength $trace_length --batch 10 --random_strategy
			./baever --MDP 3 --traces $number_traces --tracelength $trace_length --batch 10 --first_strategy	
		done
	done		
done   

