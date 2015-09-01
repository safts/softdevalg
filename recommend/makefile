CFLAGS = -g3 $(PARFLAGS)
MEDOIDOUT = medoids
RECOUT = recommend
PARFLAGS = -fopenmp

all: recommendAPP

clean:
	rm -f *.o *~ $(MEDOIDOUT) $(RECOUT) 


#cluster

CLUSTEROBJS = 	kMedoidPlusPlus.o concentrate.o pam.o LSHAssign.o claransUpdate.o lloyds.o
CLUSTERDEPS = 	./cluster/assignment/pam.h ./cluster/initialization/kMedoidPlusPlus.h \
				./cluster/initialization/concentrate.h ./cluster/update/claransUpdate.h \
				./cluster/update/lloyds.h ./cluster/assignment/LSHAssign.h $(CLUSTEROBJS)


pam.o: ./cluster/assignment/pam.c ./cluster/assignment/pam.h
	gcc -c $(CFLAGS) ./cluster/assignment/pam.c

kMedoidPlusPlus.o: ./cluster/initialization/kMedoidPlusPlus.c ./cluster/initialization/kMedoidPlusPlus.h
	gcc -c $(CFLAGS) ./cluster/initialization/kMedoidPlusPlus.c

concentrate.o: ./cluster/initialization/concentrate.c ./cluster/initialization/concentrate.h
	gcc -c $(CFLAGS) ./cluster/initialization/concentrate.c

claransUpdate.o: ./cluster/update/claransUpdate.c ./cluster/update/claransUpdate.h
	gcc -c $(CFLAGS) ./cluster/update/claransUpdate.c
	
lloyds.o: ./cluster/update/lloyds.c ./cluster/update/lloyds.h
	gcc -c $(CFLAGS) ./cluster/update/lloyds.c

LSHAssign.o: ./cluster/assignment/LSHAssign.c ./cluster/assignment/LSHAssign.h
	gcc -c $(CFLAGS) ./cluster/assignment/LSHAssign.c


#input

INPUTOBJS = dataEuclidean.o dataHamming.o dataDistanceMatrix.o dataCosine.o random.o
INPUTDEPS = ./input/dataEuclidean.h ./input/dataHamming.h ./input/dataDistanceMatrix.h \
			./input/dataCosine.h ./input/random.h $(INPUTOBJS)

dataCosine.o: ./input/dataCosine.c ./input/dataCosine.h ./dataStruct/data.h
	gcc -c $(CFLAGS) ./input/dataCosine.c

dataEuclidean.o: ./input/dataEuclidean.c ./input/dataEuclidean.h ./dataStruct/data.h
	gcc -c $(CFLAGS) ./input/dataEuclidean.c

dataHamming.o: ./input/dataHamming.c ./input/dataHamming.h ./dataStruct/data.h
	gcc -c $(CFLAGS) ./input/dataHamming.c

dataDistanceMatrix.o: ./input/dataDistanceMatrix.c ./input/dataDistanceMatrix.h ./dataStruct/data.h
	gcc -c $(CFLAGS) ./input/dataDistanceMatrix.c
	
random.o: ./input/random.c ./input/random.h 
	gcc -c $(CFLAGS) ./input/random.c 


#dataStruct

DATASTROBJS = data.o hashTable.o list.o
DATASTRDEPS = ./dataStruct/data.h ./dataStruct/hashTable.h ./dataStruct/list.h $(DATASTROBJS)

data.o: ./dataStruct/data.c ./dataStruct/data.h
	gcc -c $(CFLAGS) ./dataStruct/data.c

hashTable.o: ./dataStruct/hashTable.c ./dataStruct/hashTable.h
	gcc -c $(CFLAGS) ./dataStruct/hashTable.c

list.o: ./dataStruct/list.c ./dataStruct/list.h ./dataStruct/data.h
	gcc -c $(CFLAGS) ./dataStruct/list.c


#LSH

LSHOBJS = LSH.o
LSHDEPS = ./LSH/LSH.h $(LSHOBJS)

LSH.o: ./LSH/LSH.c ./LSH/LSH.h
	gcc -c $(CFLAGS) ./LSH/LSH.c

#clusterAPP

CLUSTERAPPOBJS = $(CLUSTEROBJS) $(INPUTOBJS) $(DATASTROBJS) $(LSHOBJS) clusterMain.o
CLUSTERAPPDEPS = $(CLUSTERDEPS) $(INPUTDEPS) $(DATASTRDEPS) $(LSHDEPS) $(CLUSTERAPPOBJS)

clusterMain.o: ./clusterAPP/main.c
	gcc -c $(CFLAGS) ./clusterAPP/main.c -o clusterMain.o


#recommendAPP

RECOMMENDAPPOBJS = $(CLUSTEROBJS) $(INPUTOBJS) $(DATASTROBJS) $(LSHOBJS) recMain.o implRecommend.o
RECOMMENDAPPDEPS = $(CLUSTERDEPS) $(INPUTDEPS) $(DATASTRDEPS) $(LSHDEPS) $(RECOMMENDAPPOBJS)

recMain.o: ./main.c
	gcc -c $(CFLAGS) ./main.c -o recMain.o

implRecommend.o: ./implRecommend.c
	gcc -c $(CFLAGS) implRecommend.c

#applications

clusterAPP:	$(CLUSTERAPPDEPS)
	gcc $(CFLAGS) $(CLUSTERAPPOBJS) -lm -o medoids
	
recommendAPP:	$(RECOMMENDAPPDEPS)
	gcc $(CFLAGS) $(RECOMMENDAPPOBJS) -lm -o recommend
	
	
###################TEMP TARGETS

###################TEMP TARGETS
