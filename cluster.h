#ifndef CLUSTER
#define CLUSTER
#include <iostream>
#include "curveList.h"

class Cluster{
	int id;
	Curve center;
	int curveNumber;
	Curve *points;
public:
	Cluster();
	Cluster(int size);
	void initArray(int size);
	~Cluster();
	int getId();
	void setId(int id);
	int getCurveNumber();
	void setCurveNumber(int number);
	Curve getCenter();
	void setCenter(Curve center);
	Curve* getPoints();
	void setPoint(Curve point,int i);
	void addPoint(Curve point);
	void print();
};


#endif
