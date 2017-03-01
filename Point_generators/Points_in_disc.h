#ifndef POINTS_IN_DISC
#define POINTS_IN_DISC


#include<cstdlib>
#include<cmath>

namespace Algorithms {

template< class K> 
class Points_in_disc
{
  typedef K                              Kernel;
  typedef typename Kernel::Number_type   NT;
  typedef typename Kernel::Point         Point;

  //Member objects

  Point _c;
  NT    _r;

public:

  Points_in_disc(const Point& c=Point(0.0,0.0) , const NT& r=NT(1.0), int seed=333):_c(c),_r(r)
  { std::srand(seed);}


  template < class Outputiterator > 
  void
  operator()( Outputiterator res, int count = 1)
  {
    for(int i=0; i<count; ++i)
      *res++ = ++(*this);

    return;
  }

  Point
  operator++()
  {
     NT r = _r*NT(std::rand())/NT(RAND_MAX);
     NT rad = NT(2.0*3.141593)*NT(std::rand())/NT(RAND_MAX);
     NT x = r*std::cos(rad),
        y = r*std::sin(rad); 

     return Point(x,y);
  }


};


} // Algorithms

#endif // POINTS_IN_DISC
