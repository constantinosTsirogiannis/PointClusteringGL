#ifndef POINT_H
#define POINT_H

#include<iostream>

namespace Types {

template < class K> 
class Point
{
  typedef K                              Kernel; 
  typedef typename Kernel::Number_type   NT;

  //Member_objects

  std::pair<NT,NT>  _coords;

public:

  Point()
  {}

  Point(const NT& a, const NT& b):_coords(a,b)
  {}

  const NT&
  x(void) const
  {return _coords.first;}

  const NT&
  y(void) const
  {return _coords.second;}

};


  template <class K>
  bool
  operator<(const Point<K>& a, const Point<K>& b)
  {
    if(a.x()<b.x())
      return true;

    if(a.x()==b.x())
      return (a.y()<b.y());

    return false;
  }

  template <class K>
  bool
  operator > (const Point<K>& a, const Point<K>& b)
  {
    if(a.x()>b.x())
      return true;

    if(a.x()==b.x())
      return (a.y()>b.y());

    return false;
  }

  template <class K>
  bool
  operator == (const Point<K>& a, const Point<K>& b)
  { return ( a.x() == b.x() && a.y() == b.y() );}


  template <class K>
  bool
  operator != (const Point<K>& a, const Point<K>& b)
  { return !(a==b);}


  template <class K>
  bool
  operator <= (const Point<K>& a, const Point<K>& b)
  {
    if(a.x()<b.x())
      return true;

    if(a.x()==b.x())
      return (a.y()<=b.y());

    return false;
  }

  template <class K>
  bool
  operator >= (const Point<K>& a, const Point<K>& b)
  {
    if(a.x()>b.x())
      return true;

    if(a.x()==b.x())
      return (a.y()>=b.y());

    return false;
  }

  template <class K>
  std::ostream & operator << (std::ostream& os, const Point<K>& p) 
  {
    os << "(" << p.x() << "," << p.y() << ")" ;
    return os;
  }

} //Types


#endif // POINT_H
