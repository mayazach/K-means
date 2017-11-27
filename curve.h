#ifndef CURVE
#define CURVE

/**
	Curve object to be stored and search with LSH.
	points = all the points in Rn space that make up the curve
	m = number of points for this curve
	dimension = the value of n in Rn
	id = name or number that identifies the curve
**/
struct Curve{
	int m;
	std::string id;
	int dimension;
	double** points;
	bool assigned;
	int clusterId;
};

/**
	This object stores the results of a nearest neighbor search.
	id = name or number of the neighbor found
	dist = the distance to the neighbor found
**/
struct Neighbor{
	double dist;
	std::string id;
};

#endif
