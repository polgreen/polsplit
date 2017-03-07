
echo "Bash version ${BASH_VERSION}..."

for i in `seq 0 10 100`;
do
   >model_param.h

	echo "#ifndef MODELPARAM_H">>model_param.h
	echo "#define MODELPARAM_H">>model_param.h
	echo "#define PARAM1 " $i >>model_param.h

	echo "#define PARAM2 " $i >>model_param.h
	echo "#endif">>model_param.h
	./compile
	./baever
done   

