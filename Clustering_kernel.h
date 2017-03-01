#ifndef CLUSTERING_KERNEL_H
#define CLUSTERING_KERNEL_H

#include<set>

#include "Cluster.h"
#include "Point.h"
#include "Distance_pair.h"
#include "Full_distance_pair.h"
#include "Tags.h"
#include "Hierarchical_algorithm.h"
#include "Point_generators/Points_in_disc.h"
#include "Aggregate_timer.h"

#include "Visuals/Dendrogram.h"
#include "Visuals/Digital_point.h"

template < class NT , class Metr_tag = typename Tag_types::Single_link >
struct Clustering_kernel
{
  typedef Clustering_kernel<NT,Metr_tag>        Self;
  typedef NT                                    Number_type;
  typedef Metr_tag                              Metric_tag;
  typedef typename Types::Point<Self>           Point;
  typedef typename Types::Cluster<Self>         Cluster;
  typedef typename Types::Distance_pair<Self>   Distance_pair;

  typedef  std::pair<Point,Point>               Constraint;
  typedef  std::set<Constraint>                 Constraint_container;

  typedef typename Tag_types::Tag_true          Tag_true;
  typedef typename Tag_types::Tag_false         Tag_false;

  typedef typename Tag_types::Single_link       Single_link;
  typedef typename Tag_types::Clique            Clique;
  typedef typename Tag_types::Average           Average;


  typedef typename Algorithms::Hierarchical_algorithm<Self,Tag_false>
                                                Hierarchical_algorithm;

  typedef typename Algorithms::Full_distance_pair<Self>
                                                Full_distance_pair;

  typedef typename Algorithms::Points_in_disc<Self>
                                                Points_in_disc;

  typedef typename Algorithms::Aggregate_timer<Self>
                                                Aggregate_timer;

  typedef typename Algorithms::Dendrogram<Self>
                                                Dendrogram;

  typedef typename Algorithms::Digital_point<Self>
                                                Digital_point;

};

#endif //CLUSTERING_KERNEL_H
