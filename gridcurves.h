#ifndef GRIDCURVES
#define GRIDCURVES

int gridify(int k,double* tvalues,Curve c,double d, char hash,int dimension);

int hash_classic(double* g,int gsize);

int hash_lsh(double* g, int gsize);

#endif