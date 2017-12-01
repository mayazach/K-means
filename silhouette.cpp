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
#include "cluster.h"
using namespace std;

double max_num(double a,double b)
{
   if(a>=b)
   {
   return a;

   }
   else
   {
   return b;
   
   }

}



double* dfd_silhouette(Cluster clusters[],int k)
{   
    /*distance='f';*/
    /*gia kathe cluster vriskei to pio kodino cluster*/
    /*gia kathe object ypologizei tis minimum apostaseis a(i) kai b(i)*/
    /*ypologizei ta s(i)*/
    /*vriskei to meso oro*/
    double* sil_table=new double[k+1];
    int cl_num=0;
    int i;
    int j;
    double s;
    

    Curve center;
    double tempDist;
    Curve tempCen;
    double minCenDist=-1;
    int nearCenNum;
    Curve nearCenCurv;
    int* neighbours;
    
    int curveNumber;
    double* a;
    double* b;
    /*exoume k kedra,ara kai k kodina kedra*/
    neighbours=new int[k];
    for(cl_num=0;cl_num<k;cl_num++)
    {
    /*cout<<"cl_num="<<cl_num<<"\n";*/
    center=clusters[cl_num].getCenter();
    minCenDist=-1;
    for(i=0;i<k;i++)
    {  
       /*elegxoume oti den prokeitai gia to idio kedro*/
       if(i!=cl_num)
       {
          /*an einai to prwto kedro pou eksetazetai,theoreitai min*/
          if(minCenDist==-1){
             nearCenCurv=clusters[i].getCenter();
            
             minCenDist=dfd(&center,&nearCenCurv);
             
             nearCenNum=i;
             
             
             
             
          }
          else{
             tempCen=clusters[i].getCenter();
             tempDist=dfd(&center,&tempCen);
             /*cout<<"minCenDist"<<minCenDist<<" ";
             cout<<"tempDist"<<tempDist<<"\n";*/
             if(tempDist<minCenDist)
             { 
               nearCenCurv=tempCen;
               minCenDist=tempDist; 
               nearCenNum=i;
             }
              
              
          }

       }
    }
    /*apothikeysh tou kodinoterou cluster sthn adistoixh thesh tou sxetikou pinaka*/
    neighbours[cl_num]=nearCenNum;
 
    }
    /*typwnw*/
    /*for(cl_num=0;cl_num<k;cl_num++)
    {   
       cout<<"center curve id"<<clusters[cl_num].getId();
       cout<<"neighbour curve id"<<clusters[neighbours[cl_num]].getId();
       cout<<"\n";

    
    }*/
   
 
    double sum;
   
    cl_num=0;
    for(cl_num=0;cl_num<k;cl_num++){
    /*ypologismos twn a*/
    a=new double[clusters[cl_num].getCurveNumber()];
    for(i=0;i<clusters[cl_num].getCurveNumber();i++)
    {  
       sum=0;
       for(j=0;j<clusters[cl_num].getCurveNumber();j++)
       {
          if(i!=j)
          {         
           sum=sum+dfd(&(clusters[cl_num].getPoints()[i]),&(clusters[cl_num].getPoints()[j]));
           /*cout<<"sum="<<sum<<"\n";*/
          }

       }
       a[i]=sum/(clusters[cl_num].getCurveNumber()-1);
       /*cout<<"a"<<a[i];*/
       
    }
    
    
    /*ypologismos twn b*/
    b=new double[clusters[cl_num].getCurveNumber()];
    for(i=0;i<clusters[cl_num].getCurveNumber();i++)
    {
        sum=0;
        for(j=0;j<clusters[neighbours[cl_num]].getCurveNumber();j++)
        {
            
          sum=sum+dfd(&(clusters[cl_num].getPoints()[i]),&(clusters[neighbours[cl_num]].getPoints()[j]));
        }
        b[i]=sum/(clusters[neighbours[cl_num]].getCurveNumber());


    }
    /*ypologismos silhouettes gia ta simeia tou cluster*/
    sum=0;
    for(i=0;i<clusters[cl_num].getCurveNumber();i++)
    {
        s=(b[i]-a[i])/(max_num(a[i],b[i]));
        /*cout<<"s="<<s<<"\n";*/
        sum=sum+s;

    }
    /*average silhouette gia 1 cluster*/
    sil_table[cl_num]=sum/(clusters[cl_num].getCurveNumber());
    /*cout<<"sil["<<cl_num<<"]="<<sil_table[cl_num]<<"\n";*/
    
   /*apodemeysh xwrou sto telos 1 epanalisphs*/
    delete[] a;
    delete[] b;
    }
   /*ypologizw to synoliko silhouette*/
   sum=0;
   for(cl_num=0;cl_num<k;cl_num++)
   {
   sum=sum+sil_table[cl_num];

   }
   sil_table[cl_num]=sum/k;
   /*to synoliko silhouete apothikeuetai sthn teleutaia thesh tou pinaka*/
   /*typwnw gia kathe cluster tis times silhouette*/
   
   /*for(cl_num=0;cl_num<k;cl_num++)
    {

    cout<<"sil["<<cl_num<<"]="<<sil_table[cl_num]<<"\n";

    }
    cout<<"synoliko silhouette sil["<<cl_num<<"]="<<sil_table[cl_num]<<"\n";*/
  
    /*apodesmeysh xwrou sto telos ths synartishs*/
   /*delete[] neighboursArray;*/
   delete[] neighbours;
  

   return sil_table;
     
 }



                 
