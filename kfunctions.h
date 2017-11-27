#ifndef KFUNCTIONS
#define KFUNCTIONS
#include <iostream>
#include "curve.h"
#include "cluster.h"
#include "hash.h"

void randomK(Curve curves[],int curveNum,Cluster clusters[],int clusterNum);

void lloydAssignment(Curve curves[],int curveNum,Cluster clusters[],int clusterNum,char distance);

void lshAssignment(hashTable* table[],int hashNum,int tablesize,int k,double d,double** tvalues,Curve curves[],int curveNum,Cluster clusters[],int clusterNum,char distance);

Curve meanFrechet(Curve curves[],int curveNum);

void pam(Cluster clusters[],int clusterNum,char distance);

#endif
