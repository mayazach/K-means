#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <limits>
#include "curve.h"
#include "pairsList.h"
using namespace std;



/*synartiseis ths pairsList*/
PairsList::PairsList(){
  head=NULL;
  size=0;


};

PairsList::~PairsList(){
      int i;
      while(head!=NULL){
           PairsNode* n=head->next;
           delete head;
           head=n;

      }



};

void PairsList::print(){
    cout<<"printing Pairslist\n";
    PairsNode *n=head;
    while(n!=NULL){
       cout<<"pair\n";
       cout<<n->point_pair.p<<"\n";
       cout<<n->point_pair.q<<"\n";
       cout<<"\n";
       n=n->next;
    }     
    cout<<"ended printing"<<"\n";



};
/*void curvePrint(Curve curve){
	int i,j;
	cout << curve.id << endl << endl;
    if(curve.points != NULL){
    	for(i=0;i<curve.m-1;i++){
			cout << "(";
                for(j=0;j<curve.dimension-1;j++)
                    	cout << curve.points[i][j] << ", ";
			cout << curve.points[i][j] << "), ";
		}
        cout << "(";
        for(j=0;j<curve.dimension-1;j++)
            cout << curve.points[i][j] << ", ";
        cout << curve.points[i][j] << ")" << endl;
		cout << endl;
	}
}
void CurveList::print(){
	CurveNode *n = head;
	while(n!=NULL){
		curvePrint(n->curve);
		n=n->next;
	}
};
*/
int PairsList::getSize(){
   return size;  

}

bool PairsList::isEmpty(){
   return (size==0);

}

void PairsList::push(Pair p){
    PairsNode *n=new PairsNode(p,head);
    head=n;
    size++;
        
}

Pair PairsList::remove(){
       PairsNode *n;
       n=head;
       head=head->next;
       Pair p=n->point_pair;
       size--;
       delete n;
       return p;



}



/*Curve CurveList::remove(){
	CurveNode *n;
	n = head;
	head = head->next;
	Curve c = n->curve;
	size--;
	delete n;
	return c;
};
*/
/*void CurveList::push(Curve c){
	CurveNode *n = new CurveNode(c,head);
	head = n;
	size++;
};
*/

