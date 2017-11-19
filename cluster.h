#ifndef CLUSTER
#define CLUSTER
#include <iostream>
#include "curveList.h"

class Cluster{
	int id;
	Curve center;
	CurveList points;
public:
	Cluster();
	~Cluster();
	int getId();
	void setId(int id);
	Curve getCenter();
	void setCenter(Curve center);
};


#endif