#include <iostream>
#include <cstdlib> 
#include <ctime> 
#include <cmath>
#include <limits>
#include "curve.h"
#include "cluster.h"
#include "distance.h"
#include "randomnum.h"

using namespace std;

/*n simeia,t clusters exoun hdh epilegei,k o synolikos arithmos twn clusters*/
/*kmeans++*/
/*arxika epilegetai tyxaio kedro*/
/*to kedro pristithetai ston pinaka twn clusters*/
/*ypologizodai kai apothikeyodai se pinaka oi apostaseis apo to kedro*/
/*diairoudai oi apostaseis me th megisth*/
/*ypologizetai h pithanothta gia thn kathe kabylh*/
/*epilegetai tyxaia alla me xrish ths pithanothtas to deytero kedro*/

void Kmeans_initialization(Curve curves[],int curveNum,Cluster clusters[],int clusterNum,char distance)
{
        /*distance:'f'->dfd*/
        /*clusterCurNum->clusters Current number*/
        int clusterCurNum=0;

        int i;
        int j;
        /*disToCen->minimum distance to some cenroid*/
        double min,temp;
        double max=0;
        double* distToCen;
        double* probability;
        double* choiceSpaces;
        Curve c;
        double distSum=0;
        /*int cluster_num;*/
        double max_value;
        double random_value;
        int center;
        /*curveNum=100;*/
        int* is_center=new int[curveNum];
        distToCen=new double[curveNum];
        probability=new double[curveNum];
        choiceSpaces=new double[curveNum];
        
        /*arxika kanena simeio den exei oristei ws kedro.Ara oles oi times ston pinaka is_center einai 0*/
        /*otan kapoia kabylh epilegetai ws kedro tote prostithe*/
        for(i=0;i<curveNum;i++)
        {
        is_center[i]=0;


        }
        for(i=0;i<curveNum;i++)
        {
        distToCen[i]=0;


        }
        for(i=0;i<curveNum;i++)
        {

        choiceSpaces[i]=0;

        }
      
       

 


        srand((unsigned)time(0));
        center=rand()%curveNum;
        clusterCurNum++;
        clusters[0].setCenter(curves[center]);
        is_center[center]=1;
      

        /*dfd*/
       /*DFD*/
       if(distance=='f'){
        while(clusterCurNum<clusterNum)
        {
        max=0;
        for(i=0;i<curveNum;i++)
        {   
            /*if(is_center[0]==0)*/
            if(is_center[i]==0)
            {
              c = clusters[0].getCenter();             
	      min = dfd(&(curves[i]),&c);
              
                
              for(j=1;j<clusterCurNum;j++){
                 c = clusters[j].getCenter();      
                 temp = dfd(&(curves[i]),&c);
                     
                 if(temp<min)
                 {   
                 min=temp;
                 }
              }
              distToCen[i]=min;
              /*if(i==0)
              {
              max=distToCen[i];

              }*/
              /*else if(distToCen[i]>max)*/
              if(distToCen[i]>max)
              {

              max=distToCen[i];

             

             }
           }
       }
       /*diairesh me max-evresh athroismatos*/
       
       distSum=0;

       for(i=0;i<curveNum;i++)
       {   
           if(is_center[i]==0)
           {
             distToCen[i]=distToCen[i]/max;
             /*cout <<"distToCen["<<i<<"]="<<distToCen[i]<<"\n";*/
             distSum=distSum+pow(distToCen[i],2);
           }
           
       }
       //cout<<"distSum="<<distSum<<"\n";
       //cout <<"okey\n";
       /*ypologismos pithanothtas*/
       for(i=0;i<curveNum;i++)
       {   
           if(is_center[i]==0)
           {
              probability[i]=pow(distToCen[i],2)/distSum;
              //cout<<"probability["<<i<<"]="<<probability[i]<<"\n";
           }
       }
       /*kathorismos diasthmatwn kai megisths timhs me vash thn pithanithta*/
       max_value=0;
       for(i=0;i<curveNum;i++)
       {    
            if(is_center[i]==0)
            {
               max_value=max_value+probability[i];
               choiceSpaces[i]=max_value;
               //cout<<"choiceSpaces["<<i<<"]="<<choiceSpaces[i]<<"\n";
            }


       }
       center=0;
       
     
       random_value=fRand(0,max_value);
       //cout<<"random_value="<<random_value<<"\n";
        /*epilogh kedrou me vash thn tyxaia timh-ginetai xrish ths analogikhs pithanothtas*/
       while(random_value>choiceSpaces[center])
       {
         center++;
         /*cout<<center<<"\n";*/
         while(is_center[center]==1)
         {
            center++;
         }

       }
       choiceSpaces[center]=0;
       //cout<<"center="<<center<<"\n";
        
       /*sto telos ths epanalipshs tha exei th thesh tou center*/
       
       /*auksish arithmou centers*/
       clusterCurNum++;
        
   
       /*thetoume 1 sthn adistoixh thesh ston pinaka is_center*/
       is_center[center]=1;
       
       clusters[clusterCurNum-1].setCenter(curves[center]);

       
       
       /*eisagoume thn kabylh ston pinaka twn clusters*/
       //cout<<"center"<<center;
       //cout<<"\n";
       //cout<<"clusterCurNum="<<clusterCurNum<<"\n";
       }
       //cout<<"curves-cluster-centers:\n";
       for(i=0;i<curveNum;i++)
       {
          if(is_center[i]==1)
          {
          //cout<<"curve"<<i<<"\n";
          }
       }
       }

       else{


       /*DTW*/
       /*DTW-vgazw times pou dimiourgoun provlima*/
       /*is_center=0->den einai kedro,alla borei na epilegei sth synexeia*/
       /*is_center=1->exei epilegei ws kedro*/
       /*is_center=0->den borei na epilegei ws kedro*/
        /*max=0;*/
        /*den arxikopoiw to max me thn prwth timh giati borei na einai inf*/
        while(clusterCurNum<clusterNum)
        {
        max=0;
        for(i=0;i<curveNum;i++)
        {   
            /*if(is_center[0]==0)*/
            if(is_center[i]==0)
            {
              c = clusters[0].getCenter();
              min = dtw(&(curves[i]),&c);
              /*cout<<"min="<<min<<"\n";*/
            
                
              for(j=1;j<clusterCurNum;j++){
                 c = clusters[j].getCenter();
                 temp = dtw(&(curves[i]),&c);
                 if(temp<min)
                 {   
                 min=temp;
                 }
              }
              /*distToCen[i]=min;*/
              
              if(isinf(min)==0)
              {
               /*cout<<"min2="<<min<<"\n";*/
               distToCen[i]=min;
                if(distToCen[i]>max)
                {
                max=distToCen[i];
                }
              }
              else
              {
              is_center[i]=2;

              }
           }
       }
       /*diairesh me max-evresh athroismatos*/
       
       distSum=0;

       for(i=0;i<curveNum;i++)
       {   
           if(is_center[i]==0)
           {
             distToCen[i]=distToCen[i]/max;
             //cout <<"distToCen["<<i<<"]="<<distToCen[i]<<"\n";
             distSum=distSum+pow(distToCen[i],2);
           }
       }
       //cout<<"distSum="<<distSum<<"\n";
       //cout <<"okey\n";
       /*ypologismos pithanothtas*/
       for(i=0;i<curveNum;i++)
       {   
           if(is_center[i]==0)
           {
              probability[i]=pow(distToCen[i],2)/distSum;
              //cout<<"probability["<<i<<"]="<<probability[i]<<"\n";
           }
       }
       /*kathorismos diasthmatwn kai megisths timhs me vash thn pithanithta*/
       max_value=0;
       for(i=0;i<curveNum;i++)
       {    
            if(is_center[i]==0)
            {
               max_value=max_value+probability[i];
               choiceSpaces[i]=max_value;
               //cout<<"choiceSpaces["<<i<<"]="<<choiceSpaces[i]<<"\n";
            }
            if(is_center[i]==2)
            {
               choiceSpaces[i]=0;
            }


       }
       center=0;
       
     
       random_value=fRand(0,max_value);
       //cout<<"random_value="<<random_value<<"\n";
        /*epilogh kedrou me vash thn tyxaia timh-ginetai xrish ths analogikhs pithanothtas*/
       while(random_value>choiceSpaces[center])
       {
         center++;
         /*cout<<center<<"\n";*/
         while(is_center[center]!=0)
         {
            center++;
         }

       }
       choiceSpaces[center]=0;
       //cout<<"center="<<center<<"\n";
        
       /*sto telos ths epanalipshs tha exei th thesh tou center*/
       
       /*auksish arithmou centers*/
       clusterCurNum++;
        
   
       /*thetoume 1 sthn adistoixh thesh ston pinaka is_center*/
       is_center[center]=1;
       
       clusters[clusterCurNum-1].setCenter(curves[center]);

       
       
       /*eisagoume thn kabylh ston pinaka twn clusters*/
       //cout<<"center"<<center;
       //cout<<"\n";
       //cout<<"clusterCurNum="<<clusterCurNum<<"\n";
       
       for(i=0;i<curveNum;i++)
       {
          if(is_center[i]==2)
          {
              is_center[i]=0;

          }


       }
       }
       //cout<<"curves-cluster-centers:\n";
       for(i=0;i<curveNum;i++)
       {
          if(is_center[i]==1)
          {
          //cout<<"curve"<<i<<"\n";
          }
       }      
       }

       
        delete [] is_center;
        delete [] distToCen;
        delete [] probability;
        delete [] choiceSpaces; 
       
}

             
                               
                      


       
       /* else{*/






        
        
        
        
        
  
