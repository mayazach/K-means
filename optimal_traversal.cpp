#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <limits>
#include "curve.h"
#include "pairsList.h"
#include "dfd_res.h"
using namespace std;


dfd_res::dfd_res(){ 
   cout<<"struct dfd_res has been created"<<"\n";
};
dfd_res::dfd_res(int m1,int m2)
{  
   int i;
  /*h 1 diastash tou pinaka D,xreiazetai gia thn apodesmeysh xwrou */
   d1=m1;
   D=new int*[(m1+1)];
   for(i=0;i<=m1;i++)
	{
	   D[i]=new int[(m2+1)];
	}
   cout<<"struct dfd_res has been created"<<"\n";


};

dfd_res::~dfd_res()
{
   int i;
   
   for(i=0;i<=d1;i++)
   {
   delete[] D[i]; 

   }
   delete[] D;


};

/*ylopoihsh algorithmou gia optimal_traversal_computation*/

PairsList* optimal_traversal(int m1,int m2,dfd_res* dfd_result)
{
   /*arxikopoihsh listas traversal-dilwseis metavlitwn*/
   PairsList* traversal=new PairsList();
   Pair tr;
   int minidx;

   tr.p=m1;
   tr.q=m2;
   traversal->push(tr);
   cout<<"added an element at traversal list\n";
   
   /*syblirwsh ths listas traversal*/
   while(((tr.p)!=0)&&((tr.q)!=0))
   {  
      /*xrisimopoioumai ton pinaka D gia na vroume ta pairs*/
      /*pou tha kanoume add sth lista*/
      minidx=dfd_result->D[tr.p][tr.q];
      if(minidx==0)
      {   
          tr.p=tr.p-1;
          traversal->push(tr);



      }
      else if(minidx==1)
      {
           
          tr.q=tr.q-1;
          traversal->push(tr);

      }
      else if(minidx==2)
      {
          tr.p=tr.p-1;
          tr.q=tr.q-1;
          traversal->push(tr);

      }
      else
      {
     
          
          tr.p=0;
          tr.q=0;


      }
   }

   return traversal;
}   

/*find the mean of curves*/
/*epistrefei kabylh pou adistoixei sth mean discete frechet curve*/
Curve* mean(PairsList* traversal,Curve p,Curve q,int dimension)
{    

     cout<<"Creating mean discrete frechet curve(mdfc)\n";
     int i,j;
     Curve* mdfc=new Curve();
     mdfc->m=traversal->getSize();
     mdfc->dimension=dimension;
     mdfc->points=new double*[(mdfc->m)];
     for(i=0;i<(mdfc->m);i++)
     {
        mdfc->points[i]=new double[mdfc->dimension];


     }
     

     
     Pair current;
     i=1;
     while(i<=(mdfc->m))
     {   
         /*cout<<"i="<<i<<"\n";*/
         current=traversal->remove();
         for(j=0;j<mdfc->dimension;j++)
         {
         mdfc->points[i-1][j]=(p.points[(current.p)-1][j]+q.points[(current.q)-1][j])/2;




          }  



         i++;
     }
       


     return mdfc;




}

Curve* get_mean_discrete(Curve*p, Curve*q,dfd_res* result)
{


PairsList* traversal=optimal_traversal(p->m,q->m,result);
Curve* mdfc=mean(traversal,*p,*q,p->dimension);
delete traversal;



return mdfc;
}



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

/*tropopoiw thn dfd wste na ypologizei*/
/*kai ton pinaka D*/
/*ta stoixeia tou D pairnoun tis tis times 0,1,2*/
/*epistrefetai ena struct to opoio ta perilamvenei*/
/*thn apostash pou dinei o dfd kai enan pinaka pou apotypwnei th diadromh*/
dfd_res* dfd_new(Curve* p,Curve* q){
	int m1=p->m;
	int m2=q->m;
	double max=0;
	double min=0; 
	int i;
	int j;
	double result_old;
	int dim=p->dimension;
        /*store the distance and the table with values*/
        /*that describe the traversal*/
        dfd_res* result=new dfd_res(m1,m2); 
    
        for(i=0;i<=m1;i++){
        for(j=0;j<=m2;j++)
        {
            result->D[i][j]=-1;
            /*cout<<result->D[i][j]<<"\n";*/
           
        }
        }



	double** C=new double*[(m1+1)];
	for(i=0;i<=m1;i++)
	{
	   C[i]=new double[(m2+1)];
	}
       
	for(i=0;i<=m1;i++){
	    for(j=0;j<=m2;j++){
			C[i][j]=max;
	    }
	}

	for(i=1;i<=m1;i++){
		for(j=1;j<=m2;j++){
			if((i==1)&&(j==1)){
                                result->D[i][j]=-1;
                                /*den yparxoune shneia pio prin*/
				max=euclidean(p->points[i-1],q->points[j-1],dim); 
			}
		    if((j>1)&&(i==1)){
                                result->D[i][j]=1;
				/*find max*/
				max=C[1][j-1];
				if(euclidean(p->points[i-1],q->points[j-1],dim)>max){
					max=euclidean(p->points[i-1],q->points[j-1],dim);
				}
		    } 
		    if((i>1)&&(j==1)){ 
				/*find max*/
                                result->D[i][j]=0;
				max=C[i-1][1];
				if(euclidean(p->points[i-1],q->points[j-1],dim)>max){
					max=euclidean(p->points[i-1],q->points[j-1],dim);
				}
		    }
		    if((i>1)&&(j>1)){
				/*find min*/
                                result->D[i][j]=0;
				min=C[i-1][j];
				if(C[i-1][j-1]<min)
				{
                                result->D[i][j]=2;
				min=C[i-1][j-1];
				}
				if(C[i][j-1]<min)
				{
                                result->D[i][j]=1;
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
	/*result_old=C[m1][m2];*/
        result->dist=C[m1][m2];
	for(i=0;i<=m1;i++){
		delete[] C[i];
	}
	delete[] C;
        return result;
	/*return result->dist;*/
}





