#/bin/bash

for fileName in uniform frontload increasing
do
	echo "*****file name: $fileName*****"
	for algorithm in 1 2 3
	do
		echo "<<<algorithm: $algorithm>>>"
		./SchedSim $fileName 3 10 $algorithm
	done
done
