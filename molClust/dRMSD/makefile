CFLAGS = -g3 -Wall #$(PARFLAGS)
MEDOIDOUT = medoids
RECOUT = recommend
LALIBS = -llapacke -lcblas -llapack

all: molConfAPP

clean:
	rm -f *.o *~ $(MEDOIDOUT) $(RECOUT) 


#cluster

CLUSTEROBJS = 	kMedoidPlusPlus.o concentrate.o pam.o claransUpdate.o lloyds.o
CLUSTERDEPS = 	./cluster/assignment/pam.h ./cluster/initialization/kMedoidPlusPlus.h \
				./cluster/initialization/concentrate.h ./cluster/update/claransUpdate.h \
				./cluster/update/lloyds.h  $(CLUSTEROBJS)

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


#input

INPUTOBJS = dataCosine.o ./input/dataMolecule.o random.o
INPUTDEPS = ./input/dataCosine.h ./input/dataMolecule.h ./input/random.h $(INPUTOBJS)

dataCosine.o: ./input/dataCosine.c ./input/dataCosine.h ./dataStruct/data.h
	gcc -c $(CFLAGS) ./input/dataCosine.c

dataMolecule.o: ./input/dataMolecule.c ./input/dataMolecule.h ./dataStruct/data.h
	gcc -c $(CFLAGS) ./input/dataMolecule.c

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


#clusterAPP

CLUSTERAPPOBJS = $(CLUSTEROBJS) $(INPUTOBJS) $(DATASTROBJS) clusterMain.o
CLUSTERAPPDEPS = $(CLUSTERDEPS) $(INPUTDEPS) $(DATASTRDEPS) $(CLUSTERAPPOBJS)

clusterMain.o: ./clusterAPP/main.c
	gcc -c $(CFLAGS) ./clusterAPP/main.c -o clusterMain.o

#recommendAPP

RECOMMENDAPPOBJS = $(CLUSTEROBJS) $(INPUTOBJS) $(DATASTROBJS) recMain.o implRecommend.o
RECOMMENDAPPDEPS = $(CLUSTERDEPS) $(INPUTDEPS) $(DATASTRDEPS) $(RECOMMENDAPPOBJS)

recMain.o: ./recommendAPP/main.c
	gcc -c $(CFLAGS) ./recommendAPP/main.c -o recMain.o

implRecommend.o: ./recommendAPP/implRecommend.c
	gcc -c $(CFLAGS) ./recommendAPP/implRecommend.c

#molConfAPP

MOLCONFAPPOBJS = $(CLUSTEROBJS) $(INPUTOBJS) $(DATASTROBJS) molConfMain.o implMolConf.o lapackeIF.o
MOLCONFAPPDEPS = $(CLUSTERDEPS) $(INPUTDEPS) $(DATASTRDEPS) $(MOLCONFAPPOBJS)

MOLCONFAPPFLAGS = $(LAPACKELIBS) $(BLASLIBS)

molConfMain.o: ./main.c
	gcc -c $(CFLAGS) ./main.c -o molConfMain.o
	
implMolConf.o: implMolConf.c
	gcc -c $(CFLAGS) implMolConf.c

lapackeIF.o: lapackeIF.c lapackeIF.h
	gcc -c $(CFLAGS) lapackeIF.c

#applications

clusterAPP:	$(CLUSTERAPPDEPS)
	gcc $(CFLAGS) $(CLUSTERAPPOBJS) -lm -o medoids
	
recommendAPP: $(RECOMMENDAPPDEPS)
	gcc $(CFLAGS) $(RECOMMENDAPPOBJS) -lm -o recommend
	
molConfAPP: $(MOLCONFAPPDEPS)
	gcc $(CFLAGS) $(MOLCONFAPPOBJS) $(MOLCONFAPPFLAGS) $(LALIBS) -lm -o molConfAPP
