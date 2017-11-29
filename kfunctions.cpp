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
			if(!curves[i].inDataset)
				continue;
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

void lshAssignment(hashTable* table[],int hashNum,int tablesize,int k,double d,double** tvalues,Curve curves[],int curveNum,Cluster clusters[],int clusterNum,char distance){
	int i,j,count,rIndex,hashValue,index,assignments = 0,clustersUpdated=5;
	double r,min,dist;
	Curve a, b, temp;
	string* rNeighbors = new string[curveNum];
	bool* updated = new bool[clusterNum];
	for(i=0;i<clusterNum;i++)
		clusters[i].setCurveNumber(0);
	for(i=0;i<curveNum;i++)
		curves[i].assigned = false;
	if(clusterNum > 1){
		a = clusters[0].getCenter();
		b = clusters[1].getCenter();
		if(distance == 'f')
			min = dfd(&a,&b);
		else
			min = dtw(&a,&b);
	}
	for(i=0;i<clusterNum;i++){
		for(j=i+1;j<clusterNum;j++){
			a = clusters[i].getCenter();
			b = clusters[j].getCenter();
			if(distance == 'f')
				dist = dfd(&a,&b);
			else
				dist = dtw(&a,&b);
			if(dist < min)
				min = dist;
		}
	}
	r = min/2;
	while((assignments < curveNum) && (clustersUpdated > 0)){
	for(i=0;i<clusterNum;i++)
		updated[i] = false;
	clustersUpdated = 0;
	for(i=0;i<clusterNum;i++){
		rIndex = 0;
		hashValue = gridify(k,tvalues[0],clusters[i].getCenter(),d,'c',a.dimension);
		index = hash_function(hashValue,tablesize);
		for(j=0;j<hashNum;j++)
			rIndex = table[j]->kadoi[index].dataList->findRNeighbors(rNeighbors,rIndex,curveNum,r,clusters[i].getCenter(),distance);
		for(j=0;j<rIndex;j++){
			for(count = 0;count<curveNum;count++)
				if(curves[count].id == rNeighbors[j]){
					if(!curves[count].assigned){
						curves[count].assigned = true;
						curves[count].clusterId = i;
						assignments++;
						updated[i] = true;
					}
					else{
						a = clusters[i].getCenter();
						b = clusters[curves[count].clusterId].getCenter();
						if(distance == 'f' && dfd(&a,&(curves[count])) < dfd(&b,&(curves[count]))){
							curves[count].clusterId = i;
							updated[i] = true;
						}
						if(distance == 'w' && dtw(&a,&curves[count]) < dtw(&b,&curves[count])){
							curves[count].clusterId = i;
							updated[i] = true;
						}
					}
				}
		}
	}
	for(i=0;i<clusterNum;i++)
		if(updated[i])
			clustersUpdated++;
	r = r*2;
	}
	/*for(i=0;i<curveNum;i++){
		cout << "Curve " << curves[i].id << " Assigned: " << curves[i].assigned;
		if(curves[i].assigned)
			cout << " Cluster: " << curves[i].clusterId << endl;
		else
			cout << endl;
	}*/
	for(i=0;i<curveNum;i++){
		if(curves[i].assigned)
			clusters[curves[i].clusterId].addPoint(curves[i]);
		else{
			if(!curves[i].inDataset)
				continue;
			temp = clusters[0].getCenter();
			if(distance == 'f')
				min = dfd(&temp,&(curves[i]));
			else
				min = dtw(&temp,&(curves[i]));
			index = 0;
			for(j=1;j<clusterNum;j++){
				temp = clusters[j].getCenter();
				if(distance == 'f')
					dist = dfd(&temp,&(curves[i]));
				else
					dist = dtw(&temp,&(curves[i]));
				if(dist < min){
					min = dist;
					index = j;
				}
			}
			clusters[index].addPoint(curves[i]);
		}
	}
	delete [] rNeighbors;
	delete [] updated;
}

Curve meanFrechet(Curve curves[],int curveNum){
	BinTree mytree;
	mytree.constructTree(curves,curveNum);
	return mytree.meanFrechet();
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
