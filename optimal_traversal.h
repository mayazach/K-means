#ifndef OPTIMAL_TRAVERSAL
#define OPTIMAL_TRAVERSAL
#include "dfd_res.h"
#include "pairsList.h"

PairsList* optimal_traversal(int,int,dfd_res*);
Curve mean(PairsList*,Curve,Curve,int);
dfd_res* dfd_new(Curve*,Curve*);
Curve get_mean_discrete_1(Curve*,Curve*,dfd_res*);
Curve get_mean_discrete(Curve*,Curve*);

#endif
