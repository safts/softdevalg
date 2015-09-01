


./inputGenerator -s 4 -N 1000
./inputGenerator -s 4 -N 10000



./main -d inputHamming_auto_1000.csv -q queriesHamming_auto_100.csv -o outputH100.txt > stats
./main -d inputHamming_auto_10000.csv -q queriesHamming_auto_1000.csv -o outputH1000.txt > stats
#./main -d inputHamming_auto_100000.csv -q queriesHamming_auto_10000.csv -o outputH10000.txt > stats
echo "data generated"
./main -d inputEuclidean_auto_1000.csv -q queriesEuclidean_auto_100.csv -o outputE100.txt > stats
echo "euclidean for 100 done0"
./main -d inputEuclidean_auto_10000.csv -q queriesEuclidean_auto_1000.csv -o outputE1000.txt > stats
echo "euclidean for 1000 done"
#./main -d inputEuclidean_auto_100000.csv -q queriesEuclidean_auto_10000.csv -o outputE10000.txt > stats

./main -d inputDistanceMatrix_auto_1000.csv -q queriesDistanceMatrix_auto_100.csv -o outputD100.txt > stats
echo "DistanceMatrix for 100 done"
./main -d inputDistanceMatrix_auto_10000.csv -q queriesDistanceMatrix_auto_1000.csv -o outputD1000.txt > stats
echo "DistanceMatrix for 1000 done"
#./main -d inputEuclidean_auto_100000.csv -q queriesEuclidean_auto_10000.csv -o outputE10000.txt > stats
#./main -d inputDistanceMatrix_auto_100000.csv -q queriesDistanceMatrix_auto_10000.csv -o outputD10000.txt > stats

gedit stats &

