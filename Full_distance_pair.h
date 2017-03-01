#ifndef FULL_DISTANCE_PAIR_H
#define FULL_DISTANCE_PAIR_H

#include<iostream>
#include<vector>
#include<algorithm>
#include<cmath>
#include<cassert>

namespace Algorithms{

template < class K >
class Full_distance_pair
{
  typedef K                               Kernel;
  typedef typename Kernel::Cluster        Cluster;
  typedef typename Kernel::Point          Point;
  typedef typename Kernel::Distance_pair  Distance_pair;
  typedef typename Kernel::Number_type    NT;
  typedef typename Kernel::Metric_tag     Metric_tag;

  typedef typename Kernel::Single_link    Single_link;
  typedef typename Kernel::Clique         Clique;
  typedef typename Kernel::Average        Average;
  
  typedef std::set<Point>                 Point_set;                 

public:

  Full_distance_pair()
  {}

  Distance_pair
  operator()(Cluster& cl1,Cluster& cl2, bool cannot_link=false)
  {
    typename Cluster::iterator it1,it2;

    std::vector<NT> distances;
      
    for(it1=cl1.begin(); it1!=cl1.end(); ++it1)
      for(it2=cl2.begin(); it2!=cl2.end(); ++it2)
        distances.push_back(_euclidean_dist(*it1,*it2));
  
    NT dist = _merge_dist(distances,Metric_tag());

    return Distance_pair(cl1,cl2,dist,cannot_link);
  }

  Distance_pair
  operator()(Point_set& ps1, Point_set& ps2, bool cannot_link)
  {
    typename Cluster::iterator it1,it2;
    typename Point_set::iterator pit1,pit2;

    std::vector<NT> distances;
    Cluster cl1,cl2;    

    for(pit1=ps1.begin(); pit1!=ps1.end(); ++pit1)
      cl1.insert(*pit1);     

    for(pit2=ps2.begin(); pit2!=ps2.end(); ++pit2)
      cl2.insert(*pit2);     
      
    for(it1=cl1.begin(); it1!=cl1.end(); ++it1)
      for(it2=cl2.begin(); it2!=cl2.end(); ++it2)
        distances.push_back(_euclidean_dist(*it1,*it2));
  
    NT dist = _merge_dist(distances,Metric_tag());

    return Distance_pair(cl1,cl2,dist,cannot_link);
  }

private:

  NT 
  _euclidean_dist( Point& p, Point& q)
  { 
    return NT(std::sqrt( std::pow(p.x()-q.x(),2)
                        +std::pow(p.y()-q.y(),2)));
  }

  NT
  _merge_dist(const std::vector<NT>& distances, const Single_link& sl) const
  { return *(std::min_element(distances.begin(),distances.end()));}

  NT
  _merge_dist(const std::vector<NT>& distances, const Clique& sl) const
  { return *(std::max_element(distances.begin(),distances.end()));}

  NT
  _merge_dist(const std::vector<NT>& distances, const Average& av) const
  {
    NT sum=NT(0.0);

    for(int i=0; i<distances.size(); ++i)
      sum+=distances[i];

    return sum/NT(distances.size());
  }

};

} //Algorithms

#endif //FULL_DISTANCE_PAIR_H
