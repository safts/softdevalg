medoids: ./assignment/*.h ./initialization/*.h ./update/*.h ./data.h data.o main.o pam.o kMedoidPlusPlus.o lloyds.o list.o hashTable.o dataHamming.o random.o dataDistanceMatrix.o dataEuclidean.o concentrate.o claransUpdate.o LSHAssign.o LSH.o
	gcc -g3  -o medoids data.o main.o pam.o kMedoidPlusPlus.o lloyds.o list.o hashTable.o dataHamming.o random.o dataDistanceMatrix.o dataEuclidean.o concentrate.o claransUpdate.o LSHAssign.o LSH.o -lm

test: ./assignment/*.h ./initialization/*.h ./update/*.h ./data.h data.o testRead.o pam.o kMedoidPlusPlus.o lloyds.o list.o hashTable.o dataHamming.o random.o dataDistanceMatrix.o dataEuclidean.o
	gcc -g3  -o test data.o testRead.o pam.o kMedoidPlusPlus.o lloyds.o list.o hashTable.o dataHamming.o random.o dataDistanceMatrix.o dataEuclidean.o -lm -lcunit

LSHAssign.o: ./assignment/LSHAssign.c ./assignment/LSHAssign.h
	gcc -g3 -c ./assignment/LSHAssign.c

data.o: data.c data.h
	gcc -g3 -c data.c

main.o: main.c
	gcc -g3 -c main.c

testRead.o: testRead.c
	gcc -g3 -c testRead.c

pam.o: ./assignment/pam.c ./assignment/pam.h
	gcc -g3 -c ./assignment/pam.c

kMedoidPlusPlus.o: ./initialization/kMedoidPlusPlus.c ./initialization/kMedoidPlusPlus.h
	gcc -g3 -c ./initialization/kMedoidPlusPlus.c

concentrate.o: ./initialization/concentrate.c ./initialization/concentrate.h
	gcc -g3 -c ./initialization/concentrate.c

claransUpdate.o: ./update/claransUpdate.c ./update/claransUpdate.h
	gcc -g3 -c ./update/claransUpdate.c
	
lloyds.o: ./update/lloyds.c ./update/lloyds.h
	gcc -g3 -c ./update/lloyds.c
	
random.o: random.c random.h 
	gcc -g3 -c random.c 

dataEuclidean.o: dataEuclidean.c dataEuclidean.h data.h
	gcc -g3 -c dataEuclidean.c

dataHamming.o: dataHamming.c dataHamming.h data.h
	gcc -g3 -c dataHamming.c

dataDistanceMatrix.o: dataDistanceMatrix.c dataDistanceMatrix.h data.h
	gcc -g3 -c dataDistanceMatrix.c
	
hashTable.o: hashTable.c hashTable.h
	gcc -g3 -c hashTable.c

list.o: list.c list.h data.h
	gcc -g3 -c list.c

LSH.o: LSH.c LSH.h
	gcc -g3 -c LSH.c

clean:
	rm *.o *~

