#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <cstring>
#include <cmath>
#include <sstream>
#include <limits>
#include <ctime>
#include "curveList.h"
#include "distance.h"
#include "randomnum.h"
#include "hash.h"
#include "gridcurves.h"
#include "kfunctions.h"
#include "kmeans_initialization.h"
#include "binaryTree.h"

using namespace std;

int main(int argc, char** argv){
	ifstream input; //input dataset
	ifstream config; //query dataset
	ofstream output; //output file
	bool complete = false,found = false;
	int clusters,changes;
	int k = 2, l = 3,i,j,count,dimension=2,hash_value,position;
	int n=0; //number of curves in dataset
	char func;
	CurveList mylist;//list to keep curves from input
	CurveList queryList;//list to keep curves from query file
	double** curve_t; //t values for creating grid curves
	int rn = 10; //number of max R-neighbors to return;
	int rIndex;
	/** Variables for reading file input **/
	string in;
	double coord,d = 0.0005,r=0;
	stringstream ss;
	int start,end,tablesize;
	Curve c;
	hashTable** lTables;
	Curve* curveArray;
	Cluster* clusterArray;
	BinTree* treeArray;
	string* oldCenters;
	Curve* oldCurves;

	/**
		In this section, command line input is checked and argument values are assigned
		to variables. The number of arguments must be correct, and all the required 
		flags must be present and followed by their values. The user can give the arguments 
		in any order.
	**/
	
	if(argc > 10 || argc < 9){
		cout << "Wrong number of arguments" << endl;
		return 1;
	}
	for(i=1;i<(argc-1);i++){
		if(!strcmp(argv[i],"-i")){
			found = true;
			if(argv[i+1][0] == '-'){
				cout << "Flag value missing." << endl;
				return 1;
			}
			input.open(argv[i+1]);
			break;
		}
	}
	if(!found){
		cout << "-i flag and its value are required." << endl;
		return 1;
	}
	found = false;
	for(i=1;i<(argc-1);i++){
		if(!strcmp(argv[i],"-c")){
			found = true;
			if(argv[i+1][0] == '-'){
				cout << "Flag value missing." << endl;
				return 1;
			}
			config.open(argv[i+1]);
			break;
		}
	}
	if(!found){
		cout << "-c flag and its value are required." << endl;
		return 1;
	}
	found = false;
	for(i=1;i<(argc-1);i++){
		if(!strcmp(argv[i],"-o")){
			found = true;
			if(argv[i+1][0] == '-'){
				cout << "Flag value missing." << endl;
				return 1;
			}
			output.open(argv[i+1]);
			break;
		}
	}
	if(!found){
		cout << "-o flag and its value are required." << endl;
		return 1;
	}
	found = false;
	
	for(i=1;i<(argc-1);i++){
		if(!strcmp(argv[i],"-d")){
			found = true;
			if(argv[i+1][0] == '-'){
				cout << "Flag value missing." << endl;
				return 1;
			}
			if(!strcmp(argv[i+1],"DFT"))
				func = 'f';
			else if(!strcmp(argv[i+1],"DTW"))
				func = 'w';
			else{
				cout << "-d value must be DFT or DTW" << endl;
			}
			break;
		}
	}
	if(!found){
		cout << "-d flag and its value are required." << endl;
		return 1;
	}
	found = false;
	
	if(argc > 9){
		for(i=1;i<argc;i++){
			if(!strcmp(argv[i],"-complete"))
				complete = true;
		}
		if(!complete){
			cout << "Optional flag must be -complete." << endl;
			return 1;
		}
	}
	/**
		Checking if input, output and config files opened correctly.
	**/
	if(input.fail() || config.fail() || output.fail()){
		cerr << "Error opening file" << endl;
		return 1;
	}

	/**
		In this section, the program reads the curves from the input file and adds them to 
		the list.
	**/
	
	input >> c.id;
	if(c.id == "3")
		dimension = 3;
	else if(c.id == "4")
		dimension = 4;
	else{
		c.dimension = dimension;
		input >> c.m;
		c.points = new double*[c.m];
		for(i=0;i<c.m;i++)
			c.points[i] = new double[dimension];
		for(i=0;i<c.m;i++){
			getline(input,in,')');
			replace(in.begin(),in.end(),',',' ');
			replace(in.begin(),in.end(),'(',' ');
			ss.str("");
			ss << in;
			for(j=0;j<dimension;j++){
				ss >> coord;
				c.points[i][j] = coord;
			}
			ss.clear();
		}
		c.inDataset = true;
		mylist.push(c);
	}
	//c.dimension = dimension;
	while(!input.eof()){
		input >> c.id >> c.m;
		c.points = new double*[c.m];
		for(i=0;i<c.m;i++)
			c.points[i] = new double[c.dimension];
		for(i=0;i<c.m;i++){
			getline(input,in,')');
			replace(in.begin(),in.end(),',',' ');
			replace(in.begin(),in.end(),'(',' ');
			ss.str("");
			ss << in;
			for(j=0;j<dimension;j++){
				ss >> coord;
				c.points[i][j] = coord;
			}
			ss.clear();
		}
		c.inDataset = true;
		mylist.push(c);
	}
	n = mylist.getSize(); //get number of curves in dataset
	tablesize = n/4; //Number of buckets in each hash table
	
	/**
		Reading config file
	**/
	config >> c.id >> clusters;
	config >> c.id >> k;
	config >> c.id >> l;
	
	/** Allocating memory and setting up arrays,lists and hashtables **/
	/** Creating hash tables **/
	lTables = new hashTable*[l];
	for(i=0;i<l;i++)
		lTables[i] = create_hashTable(tablesize);
	/** Setting t values for the grid curves **/
	curve_t = new double*[l];
	for(i=0;i<l;i++)
		curve_t[i] = new double[k];
	for(i=0;i<l;i++){
		for(j=0;j<k;j++)
			curve_t[i][j] = ranf(d);
	}
	curveArray = new Curve[n];
	i=0;
	while(!mylist.isEmpty()){
		c = mylist.remove();
		curveArray[i++] = c;
		for(j=0;j<l;j++){
			hash_value = gridify(k,curve_t[0],c,d,'c',dimension);
			position = hash_function(hash_value,tablesize);
			hash_insert(c,position,lTables[j]);
		}
	}
	clusterArray = new Cluster[clusters];
	for(i=0;i<clusters;i++){
		clusterArray[i].setId(i);
		clusterArray[i].initArray(n);
	}
	
	oldCenters = new string[clusters];
	oldCurves = new Curve[clusters];
	//Random-Lloyd's asignment-PAM
	changes = clusters;
	cout << "Random-LLoyd-PAM" << endl;
	randomK(curveArray,n,clusterArray,clusters);
	lloydAssignment(curveArray,n,clusterArray,clusters,func);
	for(i=0;i<clusters;i++){
			oldCenters[i] = clusterArray[i].getCenter().id;
	}
	while(changes > 0){
		changes = 0;
		pam(clusterArray,clusters,func);
		for(i=0;i<clusters;i++){
			if(clusterArray[i].getCenter().id != oldCenters[i])
				changes++;
			oldCenters[i] = clusterArray[i].getCenter().id;
		}
		cout << changes << endl;
		if(changes > 0)
			lloydAssignment(curveArray,n,clusterArray,clusters,func);
	}
	output << "Algorithm: Random-Lloyd's-PAM" << endl;
	if(func == 'f')
		output << "Metric: Frechet" << endl;
	else
		output << "Metric: DTW" << endl;
	if(!complete)
		for(i=0;i<clusters;i++)
			output << "CLUSTER-" << i+1 << " {size: " << clusterArray[i].getCurveNumber() << ", centroid: " << clusterArray[i].getCenter().id << "}" << endl;
	else
		for(i=0;i<clusters;i++){
			output << "CLUSTER-" << i+1 << " {";
			for(j=0;j<clusterArray[i].getCurveNumber()-1;j++)
				output << clusterArray[i].getPoints()[j].id << ", ";
			output << clusterArray[i].getPoints()[j].id << "}" << endl;
		}
	//Random-LSH-PAM
	changes = clusters;
	cout << "Random-LSH-PAM" << endl;
	randomK(curveArray,n,clusterArray,clusters);
	lshAssignment(lTables,l,tablesize,k,d,curve_t,curveArray,n,clusterArray,clusters,func);
	for(i=0;i<clusters;i++){
			oldCenters[i] = clusterArray[i].getCenter().id;
	}
	while(changes > 0){
		changes = 0;
		pam(clusterArray,clusters,func);
		for(i=0;i<clusters;i++){
			if(clusterArray[i].getCenter().id != oldCenters[i])
				changes++;
			oldCenters[i] = clusterArray[i].getCenter().id;
		}
		cout << changes << endl;
		if(changes > 0)
			lshAssignment(lTables,l,tablesize,k,d,curve_t,curveArray,n,clusterArray,clusters,func);
	}
	output << endl << "Algorithm: Random-LSH-PAM" << endl;
	if(func == 'f')
		output << "Metric: Frechet" << endl;
	else
		output << "Metric: DTW" << endl;
	if(!complete)
		for(i=0;i<clusters;i++)
			output << "CLUSTER-" << i+1 << " {size: " << clusterArray[i].getCurveNumber() << ", centroid: " << clusterArray[i].getCenter().id << "}" << endl;
	else
		for(i=0;i<clusters;i++){
			output << "CLUSTER-" << i+1 << " {";
			for(j=0;j<clusterArray[i].getCurveNumber()-1;j++)
				output << clusterArray[i].getPoints()[j].id << ", ";
			output << clusterArray[i].getPoints()[j].id << "}" << endl;
		}
	//K++-Lloyd-PAM
	changes = clusters;
	cout << "Kmeans++-LLoyd-PAM" << endl;
	Kmeans_initialization(curveArray,n,clusterArray,clusters,func);
	lloydAssignment(curveArray,n,clusterArray,clusters,func);
	for(i=0;i<clusters;i++){
			oldCenters[i] = clusterArray[i].getCenter().id;
	}
	while(changes > 0){
		changes = 0;
		pam(clusterArray,clusters,func);
		for(i=0;i<clusters;i++){
			if(clusterArray[i].getCenter().id != oldCenters[i])
				changes++;
			oldCenters[i] = clusterArray[i].getCenter().id;
		}
		cout << changes << endl;
		if(changes > 0)
			lloydAssignment(curveArray,n,clusterArray,clusters,func);
	}
	output << endl << "Algorithm: K++-Lloyd's-PAM" << endl;
	if(func == 'f')
		output << "Metric: Frechet" << endl;
	else
		output << "Metric: DTW" << endl;
	if(!complete)
		for(i=0;i<clusters;i++)
			output << "CLUSTER-" << i+1 << " {size: " << clusterArray[i].getCurveNumber() << ", centroid: " << clusterArray[i].getCenter().id << "}" << endl;
	else
		for(i=0;i<clusters;i++){
			output << "CLUSTER-" << i+1 << " {";
			for(j=0;j<clusterArray[i].getCurveNumber()-1;j++)
				output << clusterArray[i].getPoints()[j].id << ", ";
			output << clusterArray[i].getPoints()[j].id << "}" << endl;
		}
	//K++-LSH-PAM
	changes = clusters;
	cout << "Kmeans++-LSH-PAM" << endl;
	Kmeans_initialization(curveArray,n,clusterArray,clusters,func);
	lshAssignment(lTables,l,tablesize,k,d,curve_t,curveArray,n,clusterArray,clusters,func);
	for(i=0;i<clusters;i++){
			oldCenters[i] = clusterArray[i].getCenter().id;
	}
	while(changes > 0){
		changes = 0;
		pam(clusterArray,clusters,func);
		for(i=0;i<clusters;i++){
			if(clusterArray[i].getCenter().id != oldCenters[i])
				changes++;
			oldCenters[i] = clusterArray[i].getCenter().id;
		}
		cout << changes << endl;
		if(changes > 0)
			lshAssignment(lTables,l,tablesize,k,d,curve_t,curveArray,n,clusterArray,clusters,func);
	}
	output << endl << "Algorithm: K++-LSH-PAM" << endl;
	if(func == 'f')
		output << "Metric: Frechet" << endl;
	else
		output << "Metric: DTW" << endl;
	if(!complete)
		for(i=0;i<clusters;i++)
			output << "CLUSTER-" << i+1 << " {size: " << clusterArray[i].getCurveNumber() << ", centroid: " << clusterArray[i].getCenter().id << "}" << endl;
	else
		for(i=0;i<clusters;i++){
			output << "CLUSTER-" << i+1 << " {";
			for(j=0;j<clusterArray[i].getCurveNumber()-1;j++)
				output << clusterArray[i].getPoints()[j].id << ", ";
			output << clusterArray[i].getPoints()[j].id << "}" << endl;
		}

	if(func == 'f'){
		//Random-LLoyd-Frechet
		changes = 0;
		count = 1;
		cout << "Random-Lloyd-Frechet" << endl;
		randomK(curveArray,n,clusterArray,clusters);
		lloydAssignment(curveArray,n,clusterArray,clusters,func);
		for(i=0;i<clusters;i++){
			oldCurves[i] = clusterArray[i].getCenter();
		}
		treeArray = new BinTree[clusters];
		for(i=0;i<clusters;i++){
			treeArray[i].constructTree(clusterArray[i].getPoints(),clusterArray[i].getCurveNumber());
			clusterArray[i].setCenter(treeArray[i].meanFrechet());
			if(!equalCurves(clusterArray[i].getCenter(),oldCurves[i]))
				changes++;
		}
		for(i=0;i<clusters;i++){
			oldCurves[i] = clusterArray[i].getCenter();
		}
		lloydAssignment(curveArray,n,clusterArray,clusters,func);
		cout << changes << endl;
		delete [] treeArray;
		while(changes > 0 && count < 10){
			changes = 0;
			treeArray = new BinTree[clusters];
			for(i=0;i<clusters;i++){
				treeArray[i].constructTree(clusterArray[i].getPoints(),clusterArray[i].getCurveNumber());
				clusterArray[i].setCenter(treeArray[i].meanFrechet());
				if(!equalCurves(clusterArray[i].getCenter(),oldCurves[i]))
					changes++;
			}
			for(i=0;i<clusters;i++){
				if(!oldCurves[i].inDataset){
				for(j=0;j<oldCurves[i].m;j++)
					delete [] oldCurves[i].points[j];
				delete [] oldCurves[i].points;
				}
				oldCurves[i] = clusterArray[i].getCenter();
			}
			count++;
			cout << changes << endl;
			lloydAssignment(curveArray,n,clusterArray,clusters,func);
			delete [] treeArray;
		}
		output << endl << "Algorithm: Random-Lloyd-Frechet" << endl;
		if(func == 'f')
			output << "Metric: Frechet" << endl;
		else
			output << "Metric: DTW" << endl;
		if(!complete)
			for(i=0;i<clusters;i++){
				output << "CLUSTER-" << i+1 << " {size: " << clusterArray[i].getCurveNumber() << ", centroid: [";
				for(j=0;j<clusterArray[i].getCenter().m-1;j++){
					output << "(";
					for(count=0;count<clusterArray[i].getCenter().dimension-1;count++)
						output << clusterArray[i].getCenter().points[j][count] << ", ";
					output << clusterArray[i].getCenter().points[j][count] << "), ";
				}
				output << "(";
				for(count=0;count<clusterArray[i].getCenter().dimension-1;count++)
					output << clusterArray[i].getCenter().points[j][count] << ", ";
				output << clusterArray[i].getCenter().points[j][count] << ")]" << endl;;
			}
		else
			for(i=0;i<clusters;i++){
				output << "CLUSTER-" << i+1 << " {";
				for(j=0;j<clusterArray[i].getCurveNumber()-1;j++)
					output << clusterArray[i].getPoints()[j].id << ", ";
				output << clusterArray[i].getPoints()[j].id << "}" << endl;
			}
			
		for(i=0;i<clusters;i++){
			c = clusterArray[i].getCenter();
			if(!c.inDataset){
			for(j=0;j<c.m;j++)
				delete [] c.points[j];
			delete [] c.points;
			}
		}
		//Random-LSH-Frechet
		changes = 0;
		count = 1;
		cout << "Random-LSH-Frechet" << endl;
		randomK(curveArray,n,clusterArray,clusters);
		lshAssignment(lTables,l,tablesize,k,d,curve_t,curveArray,n,clusterArray,clusters,func);
		for(i=0;i<clusters;i++){
			oldCurves[i] = clusterArray[i].getCenter();
		}
		treeArray = new BinTree[clusters];
		for(i=0;i<clusters;i++){
			treeArray[i].constructTree(clusterArray[i].getPoints(),clusterArray[i].getCurveNumber());
			clusterArray[i].setCenter(treeArray[i].meanFrechet());
			if(!equalCurves(clusterArray[i].getCenter(),oldCurves[i]))
				changes++;
		}
		for(i=0;i<clusters;i++){
			oldCurves[i] = clusterArray[i].getCenter();
		}
		lshAssignment(lTables,l,tablesize,k,d,curve_t,curveArray,n,clusterArray,clusters,func);
		cout << changes << endl;
		delete [] treeArray;
		while(changes > 0 && count < 10){
			changes = 0;
			treeArray = new BinTree[clusters];
			for(i=0;i<clusters;i++){
				treeArray[i].constructTree(clusterArray[i].getPoints(),clusterArray[i].getCurveNumber());
				clusterArray[i].setCenter(treeArray[i].meanFrechet());
				if(!equalCurves(clusterArray[i].getCenter(),oldCurves[i]))
					changes++;
			}
			for(i=0;i<clusters;i++){
				if(!oldCurves[i].inDataset){
				for(j=0;j<oldCurves[i].m;j++)
					delete [] oldCurves[i].points[j];
				delete [] oldCurves[i].points;
				}
				oldCurves[i] = clusterArray[i].getCenter();
			}
			count++;
			cout << changes << endl;
			lshAssignment(lTables,l,tablesize,k,d,curve_t,curveArray,n,clusterArray,clusters,func);
			delete [] treeArray;
		}
		output << endl << "Algorithm: Random-LSH-Frechet" << endl;
		if(func == 'f')
			output << "Metric: Frechet" << endl;
		else
			output << "Metric: DTW" << endl;
		if(!complete)
			for(i=0;i<clusters;i++){
				output << "CLUSTER-" << i+1 << " {size: " << clusterArray[i].getCurveNumber() << ", centroid: [";
				for(j=0;j<clusterArray[i].getCenter().m-1;j++){
					output << "(";
					for(count=0;count<clusterArray[i].getCenter().dimension-1;count++)
						output << clusterArray[i].getCenter().points[j][count] << ", ";
					output << clusterArray[i].getCenter().points[j][count] << "), ";
				}
				output << "(";
				for(count=0;count<clusterArray[i].getCenter().dimension-1;count++)
					output << clusterArray[i].getCenter().points[j][count] << ", ";
				output << clusterArray[i].getCenter().points[j][count] << ")]" << endl;;
			}
		else
			for(i=0;i<clusters;i++){
				output << "CLUSTER-" << i+1 << " {";
				for(j=0;j<clusterArray[i].getCurveNumber()-1;j++)
					output << clusterArray[i].getPoints()[j].id << ", ";
				output << clusterArray[i].getPoints()[j].id << "}" << endl;
			}
			
		for(i=0;i<clusters;i++){
			c = clusterArray[i].getCenter();
			if(!c.inDataset){
			for(j=0;j<c.m;j++)
				delete [] c.points[j];
			delete [] c.points;
			}
		}
		//Kmeans++-LLoyd-Frechet
		changes = 0;
		count = 1;
		cout << "K++-Lloyd-Frechet" << endl;
		Kmeans_initialization(curveArray,n,clusterArray,clusters,func);
		lloydAssignment(curveArray,n,clusterArray,clusters,func);
		for(i=0;i<clusters;i++){
			oldCurves[i] = clusterArray[i].getCenter();
		}
		treeArray = new BinTree[clusters];
		for(i=0;i<clusters;i++){
			treeArray[i].constructTree(clusterArray[i].getPoints(),clusterArray[i].getCurveNumber());
			clusterArray[i].setCenter(treeArray[i].meanFrechet());
			if(!equalCurves(clusterArray[i].getCenter(),oldCurves[i]))
				changes++;
		}
		for(i=0;i<clusters;i++){
			oldCurves[i] = clusterArray[i].getCenter();
		}
		lloydAssignment(curveArray,n,clusterArray,clusters,func);
		cout << changes << endl;
		delete [] treeArray;
		while(changes > 0 && count < 10){
			changes = 0;
			treeArray = new BinTree[clusters];
			for(i=0;i<clusters;i++){
				treeArray[i].constructTree(clusterArray[i].getPoints(),clusterArray[i].getCurveNumber());
				clusterArray[i].setCenter(treeArray[i].meanFrechet());
				if(!equalCurves(clusterArray[i].getCenter(),oldCurves[i]))
					changes++;
			}
			for(i=0;i<clusters;i++){
				if(!oldCurves[i].inDataset){
				for(j=0;j<oldCurves[i].m;j++)
					delete [] oldCurves[i].points[j];
				delete [] oldCurves[i].points;
				}
				oldCurves[i] = clusterArray[i].getCenter();
			}
			count++;
			cout << changes << endl;
			lloydAssignment(curveArray,n,clusterArray,clusters,func);
			delete [] treeArray;
		}
		output << endl << "Algorithm: K++-Lloyd-Frechet" << endl;
		if(func == 'f')
			output << "Metric: Frechet" << endl;
		else
			output << "Metric: DTW" << endl;
		if(!complete)
			for(i=0;i<clusters;i++){
				output << "CLUSTER-" << i+1 << " {size: " << clusterArray[i].getCurveNumber() << ", centroid: [";
				for(j=0;j<clusterArray[i].getCenter().m-1;j++){
					output << "(";
					for(count=0;count<clusterArray[i].getCenter().dimension-1;count++)
						output << clusterArray[i].getCenter().points[j][count] << ", ";
					output << clusterArray[i].getCenter().points[j][count] << "), ";
				}
				output << "(";
				for(count=0;count<clusterArray[i].getCenter().dimension-1;count++)
					output << clusterArray[i].getCenter().points[j][count] << ", ";
				output << clusterArray[i].getCenter().points[j][count] << ")]" << endl;;
			}
		else
			for(i=0;i<clusters;i++){
				output << "CLUSTER-" << i+1 << " {";
				for(j=0;j<clusterArray[i].getCurveNumber()-1;j++)
					output << clusterArray[i].getPoints()[j].id << ", ";
				output << clusterArray[i].getPoints()[j].id << "}" << endl;
			}
			
		for(i=0;i<clusters;i++){
			c = clusterArray[i].getCenter();
			if(!c.inDataset){
			for(j=0;j<c.m;j++)
				delete [] c.points[j];
			delete [] c.points;
			}
		}
		//K++-LSH-Frechet
		changes = 0;
		count = 1;
		cout << "K++-LSH-Frechet" << endl;
		Kmeans_initialization(curveArray,n,clusterArray,clusters,func);
		lshAssignment(lTables,l,tablesize,k,d,curve_t,curveArray,n,clusterArray,clusters,func);
		for(i=0;i<clusters;i++){
			oldCurves[i] = clusterArray[i].getCenter();
		}
		treeArray = new BinTree[clusters];
		for(i=0;i<clusters;i++){
			treeArray[i].constructTree(clusterArray[i].getPoints(),clusterArray[i].getCurveNumber());
			clusterArray[i].setCenter(treeArray[i].meanFrechet());
			if(!equalCurves(clusterArray[i].getCenter(),oldCurves[i]))
				changes++;
		}
		for(i=0;i<clusters;i++){
			oldCurves[i] = clusterArray[i].getCenter();
		}
		lshAssignment(lTables,l,tablesize,k,d,curve_t,curveArray,n,clusterArray,clusters,func);
		cout << changes << endl;
		delete [] treeArray;
		while(changes > 0 && count < 10){
			changes = 0;
			treeArray = new BinTree[clusters];
			for(i=0;i<clusters;i++){
				treeArray[i].constructTree(clusterArray[i].getPoints(),clusterArray[i].getCurveNumber());
				clusterArray[i].setCenter(treeArray[i].meanFrechet());
				if(!equalCurves(clusterArray[i].getCenter(),oldCurves[i]))
					changes++;
			}
			for(i=0;i<clusters;i++){
				if(!oldCurves[i].inDataset){
				for(j=0;j<oldCurves[i].m;j++)
					delete [] oldCurves[i].points[j];
				delete [] oldCurves[i].points;
				}
				oldCurves[i] = clusterArray[i].getCenter();
			}
			count++;
			cout << changes << endl;
			lshAssignment(lTables,l,tablesize,k,d,curve_t,curveArray,n,clusterArray,clusters,func);
			delete [] treeArray;
		}
		output << endl << "Algorithm: K++-LSH-Frechet" << endl;
		if(func == 'f')
			output << "Metric: Frechet" << endl;
		else
			output << "Metric: DTW" << endl;
		if(!complete)
			for(i=0;i<clusters;i++){
				output << "CLUSTER-" << i+1 << " {size: " << clusterArray[i].getCurveNumber() << ", centroid: [";
				for(j=0;j<clusterArray[i].getCenter().m-1;j++){
					output << "(";
					for(count=0;count<clusterArray[i].getCenter().dimension-1;count++)
						output << clusterArray[i].getCenter().points[j][count] << ", ";
					output << clusterArray[i].getCenter().points[j][count] << "), ";
				}
				output << "(";
				for(count=0;count<clusterArray[i].getCenter().dimension-1;count++)
					output << clusterArray[i].getCenter().points[j][count] << ", ";
				output << clusterArray[i].getCenter().points[j][count] << ")]" << endl;;
			}
		else
			for(i=0;i<clusters;i++){
				output << "CLUSTER-" << i+1 << " {";
				for(j=0;j<clusterArray[i].getCurveNumber()-1;j++)
					output << clusterArray[i].getPoints()[j].id << ", ";
				output << clusterArray[i].getPoints()[j].id << "}" << endl;
			}
			
		for(i=0;i<clusters;i++){
			c = clusterArray[i].getCenter();
			if(!c.inDataset){
			for(j=0;j<c.m;j++)
				delete [] c.points[j];
			delete [] c.points;
			}
		}
	}
	//Cleanup
	for(i=0;i<n;i++){
		for(j=0;j<curveArray[i].m;j++)
			delete [] curveArray[i].points[j];
		delete [] curveArray[i].points;
	}
	delete [] curveArray;
	
	delete [] clusterArray;
	delete [] oldCenters;
	delete [] oldCurves;
	for(i=0;i<l;i++)
		destroy_hashTable(lTables[i]);
	delete [] lTables;
	for(i=0;i<l;i++)
		delete [] curve_t[i];
	delete [] curve_t;
	
	
	/** Closing files **/
	input.close();
	output.close();
	config.close();
}
