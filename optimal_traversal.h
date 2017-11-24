#ifndef OPTIMAL_TRAVERSAL
#define OPTIMAL_TRAVERSAL

PairsList* optimal_traversal(int,int,dfd_res*);
Curve* mean(PairsList*,Curve,Curve,int);
dfd_res* dfd_new(Curve*,Curve*);
Curve* get_mean_discrete(Curve*,Curve*,dfd_res*);

#endif