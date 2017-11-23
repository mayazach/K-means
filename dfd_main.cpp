#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <limits>
#include "curve.h"
#include "pairsList.h"
#include "dfd_res.h"
#include "optimal_traversal.h"

using namespace std;

/*dokimastikh main pou 1.ypologizei to dfd kai 2.ypologizei th mean discrete frechet curve*/
int main()
{

int i;
  int j;
  


 
  Curve* p=new Curve();
  p->m=2;
  p->dimension=2;
  p->points=new double*[(p->m)];
  for(i=0;i<(p->m);i++)
  {
     p->points[i]=new double[p->dimension];
  
  }

 
  Curve* q =new Curve();
  q->m=2;
  q->dimension=2;
  q->points=new double*[(q->m)];
  for(i=0;i<(q->m);i++)
  {
     q->points[i]=new double[p->dimension];


  }


  p->points[0][0]=0;
  p->points[0][1]=1;
  p->points[1][0]=1;
  p->points[1][1]=1;

  q->points[0][0]=0;
  q->points[0][1]=2;
  q->points[1][0]=1;
  q->points[1][1]=4;

  cout<<"klish dfd\n";
  dfd_res* result;



  result=dfd_new(p,q);
  cout<<"print again\n";

  for(i=1;i<=(p->m);i++){
        for(j=1;j<=(q->m);j++)
        {
        cout<<"i="<<i<<"j="<<j<<"  "<<"D="<<result->D[i][j]<<"\n";
           
        }
  }
 

  Curve* mdfc=get_mean_discrete(p,q,result);
  cout<<"printing mdfc\n";
  cout<<"mdfc has  "<<mdfc->m<<"  points"<<"\n";
  int a=0;
  int b=0;
  for(a=0;a<mdfc->m;a++)
  {  
     cout<<"point num "<<a<<"\n";
     for(b=0;b<(mdfc->dimension);b++)
     {
        cout<<mdfc->points[a][b]<<" ";


     }
     cout<<"\n";

   }

   cout<<"destoying struct with the dfd's result(distance-table D)\n";
   delete result;
   
   cout<<"destroying mdfc\n";
   for(i=0;i<mdfc->m;i++)
   {   
   delete[] mdfc->points[i] ;

   }
   delete[] mdfc->points;
   delete mdfc;

  
   cout<<"destroying points used in main\n";
   for(i=0;i<(p->m);i++)
   {
      delete[] p->points[i];   

   }
   delete[] p->points;
   delete p;
   for(i=0;i<(q->m);i++)
   {
      delete[] q->points[i];

   }
   delete[] q->points;
   delete q;
}









/*dokimasrikh main pou apla tha kalei th synartish*/
/*int main()
{
  int i;
  int j;
  


 
  Curve* p=new Curve();
  p->m=2;
  p->dimension=2;
  p->points=new double*[(p->m)];
  for(i=0;i<(p->m);i++)
  {
     p->points[i]=new double[p->dimension];
  
  }

 
  Curve* q =new Curve();
  q->m=2;
  q->dimension=2;
  q->points=new double*[(q->m)];
  for(i=0;i<(q->m);i++)
  {
     q->points[i]=new double[p->dimension];


  }


  p->points[0][0]=0;
  p->points[0][1]=1;
  p->points[1][0]=1;
  p->points[1][1]=1;

  q->points[0][0]=0;
  q->points[0][1]=2;
  q->points[1][0]=1;
  q->points[1][1]=4;

  cout<<"klish dfd\n";
  dfd_res* result;



  result=dfd_new(p,q);
  cout<<"print again\n";

  for(i=1;i<=(p->m);i++){
        for(j=1;j<=(q->m);j++)
        {
        cout<<"i="<<i<<"j="<<j<<"  "<<"D="<<result->D[i][j]<<"\n";
           
        }
  }
 


  PairsList* opt=optimal_traversal(2,2,result);
  cout<<"each pair in PairsList has 2 points,one at each curve.1->1st point,point at position 0\n";
  opt->print();

  Curve* mdfc=mean(opt,*p,*q,p->dimension);
  cout<<"printing mdfc\n";
  cout<<"mdfc has  "<<mdfc->m<<"  points"<<"\n";
  int a=0;
  int b=0;
  for(a=0;a<mdfc->m;a++)
  {  
     cout<<"point num "<<a<<"\n";
     for(b=0;b<(mdfc->dimension);b++)
     {
        cout<<mdfc->points[a][b]<<" ";


     }
     cout<<"\n";

   }
   
   cout<<"destroying PairsList\n";
   delete opt;

   cout<<"destoying struct with the dfd's result(distance-table D)\n";
   delete result;
   
   cout<<"destroying mdfc\n";
   for(i=0;i<mdfc->m;i++)
   {   
   delete[] mdfc->points[i] ;

   }
   delete[] mdfc->points;
   delete mdfc;

  
   cout<<"destroying points used in main\n";
   for(i=0;i<(p->m);i++)
   {
      delete[] p->points[i];   

   }
   delete[] p->points;
   delete p;
   for(i=0;i<(q->m);i++)
   {
      delete[] q->points[i];

   }
   delete[] q->points;
   delete q;
}*/
