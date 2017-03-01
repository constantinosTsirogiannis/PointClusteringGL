#include<iostream>
#include <stdio.h>
#include<vector>
#include"Clustering_kernel.h"
#include"Tags.h"
#include "file_structs.h"


typedef Tag_types::Average                      Metric_tag;
typedef Clustering_kernel<double,Metric_tag>    Kernel;
typedef Kernel::Aggregate_timer                 Aggregate_timer;
typedef Kernel::Cluster                         Cluster;
typedef Kernel::Point                           Point;
typedef Kernel::Hierarchical_algorithm          Hierarchical_algorithm;
typedef Kernel::Constraint_container            Constraint_container;

typedef Read_file<Kernel>                       Read_f;

int main(int argc, char*argv[])
{
	std::vector<Point> vec;
        Constraint_container must_link,cannot_link;
        Constraint_container::iterator   it,it2;	

	Read_f()(argv[1], vec, must_link, cannot_link);


    if(must_link.size()==0 && cannot_link.size()==0)
    {
      Hierarchical_algorithm hier(vec);     

    while(!hier.no_further_agglomeration())
    {
      std::pair<Cluster,Cluster> clpair = hier.pop_head();
      std::cout << "****************************************************" << std::endl;
      std::cout << " The clusters to be unified are " << std::endl 
                << clpair.first << std::endl
                << "***and***" << std::endl
                << clpair.second << std::endl;   
      std::cout << "****************************************************" << std::endl;
    }

    }        
    else
    {
      Hierarchical_algorithm  hier2(vec,must_link,cannot_link);

    while(!hier2.no_further_agglomeration())
    {
      std::pair<Cluster,Cluster> clpair = hier2.pop_head();
      std::cout << "****************************************************" << std::endl;
      std::cout << " The clusters to be unified are " << std::endl 
                << clpair.first << std::endl
                << "***and***" << std::endl
                << clpair.second << std::endl;   
      std::cout << "****************************************************" << std::endl;
    }
    }	


  return 0;
}
