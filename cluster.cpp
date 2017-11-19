#include <iostream>
#include "cluster.h"

using namespace std;

//Cluster::Cluster();

//Cluster::~Cluster();

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

