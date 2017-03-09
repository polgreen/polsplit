
echo "Bash version ${BASH_VERSION}..."

for i in `seq 5 5 95`;
do
   >model_param.h
	echo "#ifndef MODELPARAM_H">>model_param.h
	echo "#define MODELPARAM_H">>model_param.h
	echo "#define PARAM1 " $i >>model_param.h

	echo "#define PARAM2 " $i >>model_param.h
	echo "#endif">>model_param.h
	./compile
	
	for trace_length in `seq 10 10 200`;
	do
		for number_traces in `seq 10 50 1000`;
		do
			./baever --MDP 2 --traces $number_traces --tracelength $trace_length	
		done
	done	
done   

