OBJS	= main.o curveList.o distance.o randomnum.o hash.o gridcurves.o cluster.o kfunctions.o optimal_traversal.o pairsList.o
SOURCE	= main.cpp curveList.cpp distance.cpp randomnum.cpp hash.cpp gridcurves.cpp cluster.cpp kfunctions.cpp optimal_traversal.cpp pairsList.cpp
HEADER	= curve.h curveList.h distance.h randomnum.h hash.h gridcurves.h cluster.h kfunctions.h optimal_traversal.h dfd_res.h pairsList.h
OUT 	= kmeans
CC	= g++
FLAGS	= -c -g

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT)

main.o: main.cpp
	$(CC) $(FLAGS) main.cpp

curveList.o: curveList.cpp
	$(CC) $(FLAGS) curveList.cpp

distance.o: distance.cpp
	$(CC) $(FLAGS) distance.cpp

randomnum.o:randomnum.cpp
	$(CC) $(FLAGS) randomnum.cpp
	
hash.o: hash.cpp
	$(CC) $(FLAGS) hash.cpp
	
gridcurves.o: gridcurves.cpp
	$(CC) $(FLAGS) gridcurves.cpp
	
cluster.o: cluster.cpp
	$(CC) $(FLAGS) cluster.cpp
	
kfunctions.o: kfunctions.cpp
	$(CC) $(FLAGS) kfunctions.cpp
	
optimal_traversal.o: optimal_traversal.cpp
	$(CC) $(FLAGS) optimal_traversal.cpp
	
pairsList.o: pairsList.cpp
	$(CC) $(FLAGS) pairsList.cpp
	
clean:
	rm -f $(OBJS)
