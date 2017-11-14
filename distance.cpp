#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <limits>
#include "curve.h"
using namespace std;


/*p->point of curve a,q->point of curve b b*/
double euclidean(double* p,double* q,int dim){
	double d=0;
	int i;
	for(i=0;i<dim;i++){
	  d=d+pow((q[i]-p[i]),2.0);
	  
	}
	d=sqrt(d);
	return d;
}

double dfd(Curve* p,Curve* q){
	int m1=p->m;
	int m2=q->m;
	double max=0;
	double min=0; 
	int i;
	int j;
	double result;
	int dim=p->dimension;
	double** C=new double*[(m1+1)];
	for(i=0;i<=m1;i++)
	{
	   C[i]=new double[(m2+1)];
	}
	/*anaprosarmozw tous deiktes gia na ksekinane apo 0*/
	/* ?? */
	/*h kalytera kanw pinaka me diastaseis m1+1,m2+1*/
	/*gia na mhn allaksoun oi deiktes*/
	/*kai arxikopoiw me 0 auta pou einai sthn 0 grammh h sth 0 sthlh*/
	for(i=0;i<=m1;i++){
	    for(j=0;j<=m2;j++){
			C[i][j]=max;
	    }
	}

	for(i=1;i<=m1;i++){
		for(j=1;j<=m2;j++){
			if((i==1)&&(j==1)){
				max=euclidean(p->points[i-1],q->points[j-1],dim); 
			}
		    if((j>1)&&(i==1)){
				/*find max*/
				max=C[1][j-1];
				if(euclidean(p->points[i-1],q->points[j-1],dim)>max){
					max=euclidean(p->points[i-1],q->points[j-1],dim);
				}
		    } 
		    if((i>1)&&(j==1)){ 
				/*find max*/
				max=C[i-1][1];
				if(euclidean(p->points[i-1],q->points[j-1],dim)>max){
					max=euclidean(p->points[i-1],q->points[j-1],dim);
				}
		    }
		    if((i>1)&&(j>1)){
				/*find min*/
				min=C[i-1][j];
				if(C[i-1][j-1]<min)
				{
				min=C[i-1][j-1];
				}
				if(C[i][j-1]<min)
				{
				min=C[i][j-1];
				}
				/*find max*/
				max=min;
				if(euclidean(p->points[i-1],q->points[j-1],dim)>max)
				{
				max=euclidean(p->points[i-1],q->points[j-1],dim);
				}
				/*C[i][j]=max;*/
		    }
		  
		  C[i][j]=max;
		}
	}
	result=C[m1][m2];
	for(i=0;i<=m1;i++){
		delete[] C[i];
	}
	delete[] C;
	return result;
}

/*synartish dtw*/
double dtw(Curve* p,Curve* q){
	int m1=p->m;
	int m2=q->m;
	double min=0;
	int i;
	int j;
	double result;
	int dim=p->dimension;
	
	double** C=new double*[(m1+1)];
	for(i=0;i<=m1;i++){
	   C[i]=new double[(m2+1)];
	}

	for(i=1;i<=m1;i++){
	   C[i][0]=std::numeric_limits<double>::infinity();
	}

	for(i=1;i<=m2;i++){
	  C[0][i]=std::numeric_limits<double>::infinity();
	}
	C[0][0]=0;
	for(i=1;i<=m1;i++){
	   for(j=1;j<=m2;j++){
		   /*find min*/
		   min=C[i-1][j];
		   if(C[i][j-1]<min){
			  min=C[i-1][j];
		   }
		   if(C[i-1][j-1]<min){    
			  min=C[i-1][j-1];
		   }

		   /*find C[i][j]*/
		   C[i][j]=euclidean(p->points[i-1],q->points[j-1],dim)+min;
	   }


	}

	result=C[m1][m2];
	for(i=0;i<=m1;i++){
		delete[] C[i];
	}
	delete[] C;
	return result;

}








