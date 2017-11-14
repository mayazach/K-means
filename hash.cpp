#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <limits>
#include <cmath>
#include "curveList.h"
#include "hash.h"
/*h curveList xreiazetai gia ta bins*/
/*ousiastika h curveList tha exei ta gridcurves pou adistoixoun se ena */
/*prepei na yparxei adistoixish se akeraio->ginetai me lista akeraiwn*/

using namespace std;


/*??*/
/*k grid curves-k dianysmata*/
/*gia kathe dianysma pairnw mia synartish h*/
/*sth synexeia ypologizw to f=ri*h(p)+...*/
/*??*/


/*hash function->pairnei orisma ta h,ypologizei ta r kia th thes*/
int hash_function(int hval,int tableSize){
   /*apo thn curve tha prokypsoun gia kathe simeio */
   /*to h exei ftiaxtei na dinetai san orisma */
   /*mexri na vrethei o tropos ypologismou tou*/

   /*to M einai gnwsto*/
   /*M=pow(2,32)-5;*/
   int m = (int)pow(2.0,32.0) - 5;
   
   return hval % m % tableSize;
}

/*kataskeyh hashtable*/
hashTable* create_hashTable(int ar_kadwn){
    hashTable* hashT=new hashTable();
    hashT->positions=ar_kadwn;
    /*dimiourgia ar_kadwn*/
    int i;
   
    hashT->kadoi=new bins[ar_kadwn];
    for(i=0;i<hashT->positions;i++) 
		hashT->kadoi[i].dataList=new CurveList();   
   
    cout<<"hashTable created"<<"\n";
   
   return hashT;
}

/*diagrafh hashtable*/
void destroy_hashTable(hashTable* hashT)
{
   /*diagrafh thesewn*/
   /*diagrafh tou pinaka*/
   int i;
   /*gia kathe bin diagrafh ths curveList*/
    //for(i=0;i<hashT->positions;i++)
    //hashT->kadoi[i].dataList->~CurveList();
   /*destroy_bin*/
   delete[] hashT->kadoi;
   
   delete hashT;

}

/*eisagwgh neou curve*/
void hash_insert(Curve input,int position,hashTable* hashT){
   /*eisagwsgh sth sto hashT*/
   /*eisagwgh sthn curveList*/
   hashT->kadoi[position].dataList->push(input);  
}

/*typwnma tou hash table*/
void hash_print(hashTable* hashT){  
   int i;
   cout<<"print hash\n";
   for(i=0;i<(hashT->positions);i++)
   {
   cout<<"thesh:"<<i<<"\n";
   hashT->kadoi[i].dataList->print();
   }    
}

/*epistrofh ths listas apo curves se mia sygkeksrmenh thesh*/
CurveList* hash_take_insertions(int thesh,hashTable* hashT){
     return hashT->kadoi[thesh].dataList;  
}
