#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <limits>
#include <ctime>
#include "curveList.h"
#include "hash.h"
#define M 2
#define N 12

using namespace std;

/*omoiomorfh katanomh*/
int random_num(){
   int r=M+(rand()/(RAND_MAX+1.0))*(N-M+1);
   return r;
}
/*kanonikh katanomh*/
double ranf(double max){
	return(((double)rand())/RAND_MAX)*max;
}

double marsaglia_num(){
   double x1,y1,w;

   do{
      x1=2*ranf(1)-1;
      y1=2*ranf(1)-1;
      w=x1*x1+y1*y1;
     }while(w>=1.0||w==0);

   w=sqrt((-2.0*log(w))/w);
   return x1*w;
}