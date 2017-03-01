#ifndef DISTANCE_PAIR_H
#define DISTANCE_PAIR_H

#include<iostream>
#include<cmath>
#include<cassert>

namespace Types{

template < class K >
class Distance_pair
{
  typedef K                                    Kernel;
  typedef typename Kernel::Cluster             Cluster;
  typedef typename Kernel::Point               Point;
  typedef typename Kernel::Full_distance_pair  Full_distance_pair;
  typedef typename Kernel::Number_type         NT;
  typedef typename Kernel::Metric_tag          Metric_tag;

  typedef typename Kernel::Single_link         Single_link;
  typedef typename Kernel::Clique              Clique;
  typedef typename Kernel::Average             Average;
  
  // Member Objects  

  NT _distance; 
  std::pair< Cluster , Cluster >  _cl_pair;
  bool  _cannot_link;

public:

  Distance_pair()
  {}

  Distance_pair( Cluster& p, Cluster& q):_cannot_link(false)
  {

    _cl_pair = std::make_pair(p,q);

    if( p.size() == 1 && q.size() == 1)
      _distance = NT(std::sqrt( std::pow(p.min_point().x()-q.min_point().x(),2)
                                +std::pow(p.min_point().y()-q.min_point().y(),2)));
    else
      *this=Full_distance_pair()(p,q);
 
  }

  Distance_pair( const Cluster& p, const Cluster& q, const NT& a, const NT& b, int s1, bool cannot_link=false)
  {
    _cannot_link = cannot_link;
    _cl_pair = std::make_pair(p,q);
    _distance = _merge_dist(a,b,p.size(),s1,Metric_tag());
  }

  Distance_pair(const Cluster& p, const Cluster& q, const NT& distance, bool cannot_link)
  {
    _cl_pair = std::make_pair(p,q);
    _distance = distance;
    _cannot_link = cannot_link;
  }

  const NT&
  distance(void) const
  { return _distance;}

  bool
  cannot_link(void) const
  { return _cannot_link;}

  Cluster& 
  first() 
  { return _cl_pair.first;}

  Cluster& 
  second()
  { return _cl_pair.second;}

  const Cluster& 
  first() const
  { return _cl_pair.first;}

  const Cluster& 
  second() const
  { return _cl_pair.second;}


private:

  NT
  _merge_dist(const NT& a, const NT& b, int size1 , int size2 , const Single_link& sl) const
  { return std::min(a,b); }

  NT
  _merge_dist(const NT& a, const NT& b, int size1 , int size2 , const Clique& cl) const
  { return std::max(a,b); }

  NT
  _merge_dist(const NT& a, const NT& b, int size1 , int size2 , const Average& av) const
  { return (a*size2+b*(size1-size2))/size1; }

};

 template < class K >
 bool
 operator<( const Distance_pair<K>& a, const Distance_pair<K>& b )
 {
   if(a.cannot_link()== true && b.cannot_link()==false)
     return true;

   if(a.cannot_link()== false && b.cannot_link()==true)
     return false;

   return a.distance()> b.distance();
 }



} //Types

#endif //DISTANCE_PAIR_H
