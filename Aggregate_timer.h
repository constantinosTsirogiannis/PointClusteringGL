#ifndef AGGREGATE_TIMER_H
#define AGGREGATE_TIMER_H

#include <ctime>
#include <cassert>

namespace Algorithms {

template < class K >
class Aggregate_timer
{
  typedef K                                          Kernel;
  typedef typename Kernel::Number_type               NT;
  typedef typename Kernel::Point                     Point;
  typedef typename Kernel::Hierarchical_algorithm    Hierarchical_algorithm;
  typedef typename Kernel::Points_in_disc            Points_in_disc;

public:

  Aggregate_timer()
  {}

  std::ostream& operator()(int r1, int r2, int seed=111, std::ostream& os=std::cout)
  {
    std::vector<Point>  point_vec;

    assert(r1>0 && r2>=r1);
    
    Points_in_disc  pid(Point(0,0),NT(100.0),seed);

    for(int i=r1; i<= r2; i+=10)
    {
      double begin,end;

      point_vec.clear();
      pid(std::back_inserter(point_vec),i);

      Hierarchical_algorithm  hier(point_vec);

      begin = double(std::clock());
      hier();
      end = double(std::clock());

      os.precision(20);

      os << i << " " << (end - begin)/CLOCKS_PER_SEC << std::endl;

    }

    return os;

  }

};

} // Algorithms

#endif //AGGREGATE_TIMER_H
