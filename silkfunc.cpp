#include <iostream>
#include <cstdlib> 
#include <ctime> 
#include "curve.h"
#include "cluster.h"
#include "distance.h"
#include "binaryTree.h"
#include "hash.h"
#include "gridcurves.h"

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
	for(i=0;i<clusterNum;i++)
		clusters[i].setCurveNumber(0);
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

void pam(Cluster clusters[],int clusterNum,char distance){
	int i,j,k,minIndex;
	double min,dist;
	Curve temp,minCurve;
	if(distance == 'f')
	for(i=0;i<clusterNum;i++){
		if(clusters[i].getCurveNumber() <= 0)
			continue;
		dist = 0;
		temp = clusters[i].getCenter();
		clusters[i].setCenter(clusters[i].getPoints()[0]);
		clusters[i].setPoint(temp,0);
		temp = clusters[i].getCenter();
		for(k=0;k<clusters[i].getCurveNumber();k++){
			dist += dfd(&(temp),&(clusters[i].getPoints()[k]));
		}
		min = dist;
		minCurve = temp;
		minIndex = 0;
		for(j=1;j<clusters[i].getCurveNumber();j++){
			//cout << "Cluster " << i << " swap " << j << endl;
			dist = 0;
			clusters[i].setCenter(clusters[i].getPoints()[j]);
			clusters[i].setPoint(temp,j);
			temp = clusters[i].getCenter();
			for(k=0;k<clusters[i].getCurveNumber();k++){
				dist += dfd(&(temp),&(clusters[i].getPoints()[k]));
			}
			if(dist < min){
				min = dist;
				minCurve = temp;
				minIndex = k;
			}
		}
		//cout << "Cluster " << i << " updated" << endl;
		clusters[i].setCenter(minCurve);
		clusters[i].setPoint(temp,minIndex);
	}
	else
	for(i=0;i<clusterNum;i++){
		if(clusters[i].getCurveNumber() <= 0)
			continue;
		dist = 0;
		temp = clusters[i].getCenter();
		clusters[i].setCenter(clusters[i].getPoints()[0]);
		clusters[i].setPoint(temp,0);
		temp = clusters[i].getCenter();
		for(k=0;k<clusters[i].getCurveNumber();k++){
			dist += dtw(&(temp),&(clusters[i].getPoints()[k]));
		}
		min = dist;
		minCurve = temp;
		minIndex = 0;
		for(j=1;j<clusters[i].getCurveNumber();j++){
			//cout << "Cluster " << i << " swap " << j << endl;
			dist = 0;
			clusters[i].setCenter(clusters[i].getPoints()[j]);
			clusters[i].setPoint(temp,j);
			temp = clusters[i].getCenter();
			for(k=0;k<clusters[i].getCurveNumber();k++){
				dist += dtw(&(temp),&(clusters[i].getPoints()[k]));
			}
			if(dist < min){
				min = dist;
				minCurve = temp;
				minIndex = k;
			}
		}
		//cout << "Cluster " << i << " updated" << endl;
		clusters[i].setCenter(minCurve);
		clusters[i].setPoint(temp,minIndex);
	}
}


