#ifndef KFUNCTIONS
#define KFUNCTIONS
#include <iostream>
#include "curve.h"
#include "cluster.h"

void randomK(Curve curves[],int curveNum,Cluster clusters[],int clusterNum);

void lloydAssignment(Curve curves[],int curveNum,Cluster clusters[],int clusterNum,char distance);

#endif