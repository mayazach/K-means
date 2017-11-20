#ifndef PAIRSLIST
#define PAIRSLIST


/*zeugari simeiwn*/
/*paradeigma p=1,q=1->prwto simeio kabylhs P(simeio 0 ston pinaka),prwto simeio kabylhs Q(simeio 0 ston pinaka)*/
struct Pair{
  int p;
  int q;
  
};



/*struct PairsList*/
/*lista apo zeugaria simeiwn*/
class PairsList{
  struct PairsNode
  {
    Pair point_pair;
    PairsNode* next;
    PairsNode(Pair new_pair,PairsNode *point=NULL):point_pair(new_pair),next(point){}
  };
  PairsNode* head;
  int size;
  public:
  PairsList();
  ~PairsList();
  bool isEmpty();
  int getSize();
  void push(Pair new_pair);
  void print();
  Pair remove();
   

};

#endif
