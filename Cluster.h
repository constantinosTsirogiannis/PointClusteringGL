#ifndef CLUSTER_H
#define CLUSTER_H

#include<iostream>
#include<list>

namespace Types {

template < class K> 
class Cluster
{
  typedef K                                    Kernel; 
  typedef typename Kernel::Number_type         NT;
  typedef typename Kernel::Point               Point;

public:

  typedef std::list<Point>                     Container;
  typedef typename Container::iterator         iterator;
  typedef typename Container::const_iterator   const_iterator;


private:

  //Member_objects
  Point    _min_point;
  Container _cont;


public:

  Cluster()
  {}

  Cluster(const Point& p)
  { 
    _cont.push_back(p);
    _min_point=p;
  }

  Cluster( Cluster& cl1, Cluster& cl2)
  { 
    _min_point = cl1.min_point();

    insert(cl1);
    insert(cl2);
  }

  void
  create(Cluster cl)
  {
    _min_point = cl.min_point();

    _cont.clear();

    insert(cl);
  }


  //Access

  iterator 
  begin(void)
  { return _cont.begin(); }

  iterator
  end(void)
  { return _cont.end(); }

  const_iterator 
  begin(void) const
  { return _cont.begin(); }

  const_iterator
  end(void) const 
  { return _cont.end(); }


  const Point&
  min_point() const
  { return _min_point;}

  int
  size() const
  { return _cont.size();}

   
  //Modification

  void
  insert(Point p)
  {
    _cont.insert(_cont.end(),p);

    if( _min_point > p)
      _min_point = p;
  }

  void
  insert(Cluster& cl)
  {
    _cont.insert(_cont.end(),cl.begin(),cl.end());

    if( _min_point > cl.min_point())
      _min_point = cl.min_point();
  }


};

template < class K >
int
compare( const Cluster<K>& cl1, const Cluster<K>& cl2 )
{
  if(cl1.min_point()<cl2.min_point())
    return -1;

  if(cl1.min_point()>cl2.min_point())
    return 1;

  if(cl1.min_point()==cl2.min_point())
    return 0;
}

template < class K >
bool
operator<( const Cluster<K>& cl1, const Cluster<K>& cl2 )
{ 
  if(cl1.min_point()==cl2.min_point())
    return cl1.size() > cl2.size();

  return (cl1.min_point()<cl2.min_point());
}


template < class K >
bool
operator>( const Cluster<K>& cl1, const Cluster<K>& cl2 )
{ return (cl1.min_point()>cl2.min_point());}

template < class K >
bool
operator==( const Cluster<K>& cl1, const Cluster<K>& cl2 )
{ return (cl1.min_point()==cl2.min_point() && cl1.size() == cl2.size());}

template < class K >
bool
operator<=( const Cluster<K>& cl1, const Cluster<K>& cl2 )
{ return (cl1<cl2 || cl1==cl2) ;}

template < class K >
bool
operator>=( const Cluster<K>& cl1, const Cluster<K>& cl2 )
{ return (cl1>cl2 || cl1==cl2);}

template < class K >
bool
operator!=( const Cluster<K>& cl1, const Cluster<K>& cl2 )
{ return !(cl1==cl2);}


template <class K>
std::ostream & operator << (std::ostream& os, const Cluster<K>& cl)
{

  for(typename Cluster<K>::const_iterator it = cl.begin() ; it != cl.end() ; ++it)
    os << (*it) << std::endl ;

  return os;
}

} //Types


#endif // CLUSTER_H

