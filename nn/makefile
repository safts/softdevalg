#executables

main: main.o list.o hashTable.o dataHamming.o random.o LSH.o dataDistanceMatrix.o dataEuclidean.o dataFunction.o libdistance.so
	gcc -g3 -o main main.o list.o hashTable.o dataHamming.o random.o LSH.o dataDistanceMatrix.o dataEuclidean.o dataFunction.o -lm -ldl

inputGenerator: inputGenerator.o random.o 
	gcc -g3 -o inputGenerator inputGenerator.o random.o  -lm


#object files

inputGenerator.o: inputGenerator.c inputGenerator.h
	gcc -g3 -c inputGenerator.c  

libdistance.so: distance.c
	gcc -shared -o libdistance.so -fPIC distance.c

LSH.o: LSH.c LSH.h data.h
	gcc -g3 -c LSH.c

bruteforce.o: bruteforce.c
	gcc -g3 -c bruteforce.c

random.o: random.c random.h 
	gcc -g3 -c random.c 

dataEuclidean.o: dataEuclidean.c dataEuclidean.h data.h
	gcc -g3 -c dataEuclidean.c

dataHamming.o: dataHamming.c dataHamming.h data.h
	gcc -g3 -c dataHamming.c

dataDistanceMatrix.o: dataDistanceMatrix.c dataDistanceMatrix.h data.h
	gcc -g3 -c dataDistanceMatrix.c
	
dataFunction.o: dataFunction.c dataFunction.h data.h
	gcc -g3 -c dataFunction.c	

parseInput.o: parseInput.c data.h
	gcc -g3 -c parseInput.c

main.o: main.c LSH.h data.h
	gcc -g3 -c main.c

hashTable.o: hashTable.c hashTable.h
	gcc -g3 -c hashTable.c

list.o: list.c list.h data.h
	gcc -g3 -c list.c

parseInput: parseInput.o list.o dataHamming.o dataEuclidean.o
	gcc -g3 -o parseInput parseInput.o list.o dataHamming.o dataEuclidean.o
 


clean:
	rm *.o *~
