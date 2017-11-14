#ifndef CURVELIST
#define CURVELIST
#include <iostream>
#include "curve.h"

/**
	Takes a curve and prints out its id and vertex.
**/
void curvePrint(Curve curve);

class CurveList{

	struct CurveNode{
		Curve curve;
		CurveNode *next;
		CurveNode(Curve c,CurveNode *point=NULL):curve(c),next(point){}
	};
	CurveNode *head;
	int size;
public:
	CurveList();
	~CurveList(); //deletes the list, and frees all the space taken by the nodes' dynamic values
	bool isEmpty();
	int getSize();
	void push(Curve c); //add curve to the front of the list
	Curve remove(); //removes curve from the front of the list
	void print(); //prints out the vertices of all curves in the list
	/**
		When given a curve, this method searches the list and returns 
		its member that is closest to the curve. The distance function used 
		depends on the value of func: 'f' for Discreet Frechet Distance  
		or 'w' for Dynamic Time Warping.
	**/
	Neighbor minDist(Curve c,char func);
	/**
		Returns a number of names of neighbors withing r distance of a curve.
		found = the number of neighbors already found in previous searches
		max = the maximum number of neighbors needed
		The search will stop of after finding max-found neighbors. The distance 
		function used depends on the value of func. Each neighbor the method finds 
		is placed in rNeighbors at the index equal to found, and then found value
		is incremented.
	**/
	int findRNeighbors(std::string* rNeighbors,int found,int max,double r,Curve c,char func);
};

#endif
