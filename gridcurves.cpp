#include <iostream>
#include <cmath>
#include "curve.h"
#include "gridcurves.h"
#include "randomnum.h"
#include "distance.h"

using namespace std;

/**
	Takes a curve, converts it to k grid curves using k values of t (tvalues array) between 0 and d,
	and returns an int value to be used for hashing. hash value decided which function will be used to 
	get the integer from the grid curves. 'c' for classic or 'p' for probabilistic. The curve exists in 
	n-dimensional space (n is the dimension value given).
**/
int gridify(int k,double* tvalues,Curve c,double d, char hash,int dimension){
	Curve* gridcurves = new Curve[k];
	int size = c.m;
	int i,j,n,m;
	int duplicates = 0,gsize=0;
	int result;
	double round;
	bool same;
	
	//Preparing g vertex
	double* g = new double[size*k*dimension];
	for(i=0;i<(size*k*dimension);i++)
		g[i] = 0;
		
	//Creating grid curves
	for(i=0;i<k;i++){
		gridcurves[i].m = size;
		gridcurves[i].dimension = c.dimension;
		gridcurves[i].points = new double*[size];
		gridcurves[i].id = c.id;
		for(j=0;j<size;j++)
			gridcurves[i].points[j] = new double[dimension];
		for(j=0;j<size;j++)
			for(n=0;n<dimension;n++){
				round = fmod(c.points[j][n],d);
				if(round <= d/2){
					gridcurves[i].points[j][n] = c.points[j][n] + tvalues[i] - round;
				}
				else{
					gridcurves[i].points[j][n] = c.points[j][n] + tvalues[i] - round + d;
				}
			}
	}

	//Removing duplicates from each grid curve
	for(i=0;i<k;i++){
		duplicates = 0;
		for(j=1;j<size-duplicates;j++){
			same = true;
			for(n=0;n<dimension;n++)
				if(gridcurves[i].points[j][n] != gridcurves[i].points[j-1][n])
					same = false;
			if(same){
				duplicates++;
				for(n=j;n<size;n++){
					for(m=0;m<dimension;m++)
						gridcurves[i].points[n-1][m] = gridcurves[i].points[n][m];
				}
				for(n=0;n<dimension;n++)
					gridcurves[i].points[size-duplicates][n] = 0;
				j--;
			}
		}
		//Concatenating to make g
		for(j=0;j<size-duplicates;j++)
			for(n=0;n<dimension;n++){
				g[gsize] = gridcurves[i].points[j][n];
				gsize++;
			}
	}
	
	//Calling hash function using g as input
	if(hash == 'c')
		result = hash_classic(g,gsize);
	else
		result = hash_lsh(g,gsize);
	
	//cleanup
	for(i=0;i<k;i++){
		for(j=0;j<size;j++)
			delete [] gridcurves[i].points[j];
		delete [] gridcurves[i].points;
	}
	delete [] gridcurves;
	delete [] g;
	return result;
}

/**
	Turns g vertex with gsize points into an integer with a classic hash function.
**/
int hash_classic(double* g,int gsize){
	int* r = new int[gsize];
        int i;
        double sum=0;
        for(i=0;i<gsize;i++)
            r[i]=random_num();
        for(i=0;i<gsize;i++)
            sum=sum+r[i]*g[i];	
	delete [] r;
	return (int)sum;
}

/**
	Turns g vertex with gsize points into an integer with LSH hash function.
**/
int hash_lsh(double* g, int gsize){
	int i,j;
	int w=4,kvec=3;
	int* v = new int[gsize];
	int* r = new int[gsize];
	double* t = new double[kvec];
	int sum = 0;
	
	for(i=0;i<gsize;i++)
        r[i]=random_num();
	for(i=0;i<kvec;i++)
		t[i]=ranf(w);
	for(i=0;i<gsize;i++)
		v[i]= marsaglia_num();
	
	for(i=0;i<gsize;i++)
		for(j=0;j<kvec;j++)
			sum = (int) ((g[i]*v[i] + t[j])/w);
		
	
	delete [] v;
	delete [] r;
	delete [] t;
	return abs(sum);
}