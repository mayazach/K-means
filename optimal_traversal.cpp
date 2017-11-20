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


/*dokimasrikh main pou apla tha kalei th synartish*/
int main()
{
  int i;
  int j;
  /*int dim=2;*/


  /*dimiourgia curve p*/
  Curve* p=new Curve();
  p->m=2;
  p->dimension=2;
  p->points=new double*[(p->m)];
  for(i=0;i<(p->m);i++)
  {
     p->points[i]=new double[p->dimension];
  
  }

  /*dimiourgia curve q*/
  Curve* q =new Curve();
  q->m=2;
  q->dimension=2;
  q->points=new double*[(q->m)];
  for(i=0;i<(q->m);i++)
  {
     q->points[i]=new double[p->dimension];


  }

  /*1	7	(-6.4227999999999996, 53.288000000000004), (-6.4257099999999996, 53.289299999999997), (-6.4268099999999997, 53.290300000000002), (-6.4268099999999997, 53.290300000000002), 
     (-6.4268099999999997, 53.290300000000002), (-6.4271699999999994, 53.290599999999998), (-6.4271699999999994, 53.290599999999998)*/

   /*dinodai times sta simeia twn cabilwn*/
  p->points[0][0]=0;
  p->points[0][1]=1;
  p->points[1][0]=1;
  p->points[1][1]=1;

  q->points[0][0]=0;
  q->points[0][1]=2;
  q->points[1][0]=1;
  q->points[1][1]=4;

  cout<<"klish dfd\n";
  /*double result*/
  dfd_res* result;



  /*KLISH dfd_new*/
  result=dfd_new(p,q);
  cout<<"print again\n";

  /*TYPWNETAI O PINAKAS D*/
  for(i=1;i<=(p->m);i++){
        for(j=1;j<=(q->m);j++)
        {
        cout<<"i="<<i<<"j="<<j<<"  "<<"D="<<result->D[i][j]<<"\n";
           
        }
  }
  /*cout<<"result:"<<result<<"\n";*/


  /*DIMIOURGEITAI H LISTA*/
  PairsList* opt=optimal_traversal(2,2,result);
  cout<<"each pair in PairsList has 2 points,one at each curve.1->1st point,point at position 0\n";
  opt->print();

  /*kataskeyh ths mean discete curve*/
  /*xrisimopoieitai h lista gia na kathoristoun ta simeia twn kabilwn*/ 
  /*pou tha xrisimopoihthoun*/
  /*Curve* mean(PairsList* traversal,Curve p,Curve q,int dimension)*/
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
   /*apodesmeysh xwrou*/
   
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

   /*apodesmeysh xwrou*/
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


