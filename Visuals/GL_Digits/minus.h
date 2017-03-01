#ifndef minus_H
#define minus_H

#include <cstdlib>
#include <iostream>
#include "hor_particle_u.h"
#include "hor_particle_d.h"
#include "vert_particle_r.h"
#include "vert_particle_l.h"
  
  namespace Digits {

template < class _NT >
class minus {

  typedef _NT    NT;

  public:
 
  minus(){}

  void
  operator()(NT s ,NT posx,NT posy)
  {
    NT s1=s,s2=NT(2.0/3.0)*s;

    Digits::vert_particle_l<NT>()(s,posx+1.4*s,posy);
  }

};

} //minus

#endif //minus_H
