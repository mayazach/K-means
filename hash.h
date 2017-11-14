#ifndef HASH
#define HASH

struct bins{
  CurveList* dataList;
  int numData;

};

struct hashTable{
   int positions;
   bins* kadoi;
};

int hash_function(int hval,int tableSize);
hashTable* create_hashTable(int);
void destroy_hashTable(hashTable*);
void hash_insert(Curve input,int position,hashTable* hashT);
void hash_print(hashTable*);
CurveList* hash_take_insertions(int,hashTable*);

#endif