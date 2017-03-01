#include<iostream>
#include<cstdlib>
#include"Clustering_kernel.h"
#include"Tags.h"

typedef Tag_types::Average                      Metric_tag;
typedef Clustering_kernel<double,Metric_tag>    Kernel;
typedef Kernel::Aggregate_timer                 Aggregate_timer;


int main(int argc, char* argv[])
{
  Aggregate_timer aggr_t;

  int a,b,seed=111;

  a = std::atoi(argv[1]); 
  b = std::atoi(argv[2]); 

  if(argc>=3)
    seed = std::atoi(argv[3]); 

  aggr_t(a,b,seed);

  return 0;
}

