#include <iostream>
#include <cstdlib> 
#include <ctime> 
#include "curve.h"
#include "cluster.h"
#include "distance.h"

using namespace std;

void randomK(Curve curves[],int curveNum,Cluster clusters[],int clusterNum){
	srand((unsigned)time(0));
	int i;
	for(i=0;i<clusterNum;i++){
		clusters[i].setCenter(curves[rand()%curveNum]);
	}
}

void lloydAssignment(Curve curves[],int curveNum,Cluster clusters[],int clusterNum,char distance){
	int i,j,cluster;
	double min,temp;
	Curve c;
	if(distance == 'f'){
		for(i=0;i<curveNum;i++){
			cluster = 0;
			c = clusters[0].getCenter();
			min = dfd(&(curves[i]),&c);
			for(j=1;j<clusterNum;j++){
				c = clusters[j].getCenter();
				temp = dfd(&(curves[i]),&c);
				if(temp < min){
					min = temp;
					cluster = j;
				}
			}
			clusters[cluster].addPoint(curves[i]);
		}
	}
	else{
		for(i=0;i<curveNum;i++){
			cluster = 0;
			c = clusters[0].getCenter();
			min = dtw(&(curves[i]),&c);
			for(j=1;j<clusterNum;j++){
				c = clusters[j].getCenter();
				temp = dtw(&(curves[i]),&c);
				if(temp < min){
					min = temp;
					cluster = j;
				}
			}
			clusters[cluster].addPoint(curves[i]);
		}
	}
}