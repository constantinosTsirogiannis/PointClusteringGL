#ifndef D1_H
#define D1_H

#include <cstdlib>
#include <iostream>
#include "hor_particle_u.h"
#include "hor_particle_d.h"
#include "vert_particle_r.h"
#include "vert_particle_l.h"
  
  namespace Digits {

template < class _NT >
class d1 {

  typedef _NT    NT;

  public:
 
  d1(){}

  void
  operator()(NT s ,NT posx,NT posy)
  {
    NT s1=s,s2=NT(2.0/3.0)*s;

    Digits::hor_particle_u<NT>()(s,posx,posy+s);
    Digits::hor_particle_u<NT>()(s,posx+1.4*s,posy+s);
  }

};

} // d1

#endif //D1_H
