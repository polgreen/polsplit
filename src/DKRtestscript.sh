
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
	

	/tmp/baever --MDP 3 --traces 2 --tracelength 20 --batch 100 --explicit_strategy
	/tmp/baever --MDP 3 --traces 2 --tracelength 20 --batch 100 --random_strategy	

	/tmp/baever --MDP 3 --traces 4 --tracelength 10 --batch 100 --explicit_strategy
	/tmp/baever --MDP 3 --traces 4 --tracelength 10 --batch 100 --random_strategy	

	/tmp/baever --MDP 3 --traces 5 --tracelength 8 --batch 100 --explicit_strategy
	/tmp/baever --MDP 3 --traces 5 --tracelength 8 --batch 100 --random_strategy	

	/tmp/baever --MDP 3 --traces 8 --tracelength 5 --batch 100 --explicit_strategy
	/tmp/baever --MDP 3 --traces 8 --tracelength 5 --batch 100 --random_strategy	
	/tmp/baever --MDP 3 --traces 10 --tracelength 2 --batch 100 --explicit_strategy
	/tmp/baever --MDP 3 --traces 10 --tracelength 2 --batch 100 --random_strategy	

done   

