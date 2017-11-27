#include <iostream>
#include "cluster.h"
#include "curveList.h"

using namespace std;

Cluster::Cluster(){
	curveNumber = 0;
	points = NULL;
}

Cluster::Cluster(int size){
	curveNumber = 0;
	points = new Curve[size];
}

Cluster::~Cluster(){
	if(points != NULL)
		delete [] points;
}

void Cluster::initArray(int size){
	points = new Curve[size];
}

int Cluster::getId(){
	return this->id;
}

void Cluster::setId(int id){
	this->id = id;
}

int Cluster::getCurveNumber(){
	return this->curveNumber;
}

void Cluster::setCurveNumber(int number){
	this->curveNumber = number;
}

Curve Cluster::getCenter(){
	return this->center;
}

void Cluster::setCenter(Curve center){
	this->center = center;
}

Curve* Cluster::getPoints(){
	return this->points;
}

void Cluster::setPoint(Curve point,int i){
	if(i < curveNumber)
		this->points[i] = point;
}

void Cluster::addPoint(Curve point){
	this->points[curveNumber] = point;
	curveNumber++;
}

void Cluster::print(){
	int i;
	cout << "Cluster: " << this->id << endl;
	cout << "Center: " << endl;
	for(i=0;i<curveNumber;i++)
		curvePrint(this->points[i]);
}

