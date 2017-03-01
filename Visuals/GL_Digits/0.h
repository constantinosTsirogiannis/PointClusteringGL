#ifndef D0_H
#define D0_H

#include <cstdlib>
#include <iostream>
#include "hor_particle_u.h"
#include "hor_particle_d.h"
#include "vert_particle_r.h"
#include "vert_particle_l.h"
  
  namespace Digits {

template < class _NT >
class d0 {

  typedef _NT    NT;

  public:
 
  d0(){}

  void
  operator()(NT s ,NT posx,NT posy)
  {
    NT s1=s,s2=NT(2.0/3.0)*s;

    Digits::hor_particle_u<NT>()(s,posx,posy+s);
    Digits::hor_particle_d<NT>()(s,posx,posy);
    Digits::vert_particle_l<NT>()(s,posx,posy);
    Digits::hor_particle_u<NT>()(s,posx+1.4*s,posy+s);
    Digits::hor_particle_d<NT>()(s,posx+1.4*s,posy);
    Digits::vert_particle_r<NT>()(s,posx+2.9*s,posy+NT(1.0/12.0));
  }

};

} // d0

#endif //D0_H
