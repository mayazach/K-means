#include <iostream>
#include "cluster.h"
#include "curveList.h"

using namespace std;

Cluster::Cluster(){
	
}

Cluster::~Cluster(){
	
}

int Cluster::getId(){
	return this->id;
}

void Cluster::setId(int id){
	this->id = id;
}

Curve Cluster::getCenter(){
	return this->center;
}

void Cluster::setCenter(Curve center){
	this->center = center;
}

void Cluster::addPoint(Curve point){
	this->points.push(point);
}

void Cluster::print(){
	cout << "Cluster: " << this->id << endl;
	cout << "Center: " << endl;
	curvePrint(this->center);
	this->points.print();
}

