#ifndef D3_H
#define D3_H

#include <cstdlib>
#include <iostream>
#include "hor_particle_u.h"
#include "hor_particle_d.h"
#include "vert_particle_r.h"
#include "vert_particle_l.h"
  
  namespace Digits {

template < class _NT >
class d3 {

  typedef _NT    NT;

  public:
 
  d3(){}

  void
  operator()(NT s ,NT posx,NT posy)
  {
    NT s1=s,s2=NT(2.0/3.0)*s;

    Digits::hor_particle_u<NT>()(s,posx,posy+s);
    Digits::vert_particle_l<NT>()(s,posx,posy);
    Digits::hor_particle_u<NT>()(s,posx+1.4*s,posy+s);
    Digits::vert_particle_r<NT>()(s,posx+2.9*s,posy+NT(1.0/12.0));
    Digits::vert_particle_l<NT>()(s,posx+1.4*s,posy);
  }

};

} // d3

#endif //D3_H
