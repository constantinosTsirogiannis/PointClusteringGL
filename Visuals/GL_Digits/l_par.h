#ifndef L_PAR_H
#define L_PAR_H

#include <cstdlib>
#include <iostream>
#include "hor_particle_u.h"
#include "hor_particle_d.h"
#include "vert_particle_r.h"
#include "vert_particle_l.h"
  
  namespace Digits {

template < class _NT >
class l_par {

  typedef _NT    NT;

  public:
 
  l_par(){}

  void
  operator()(NT s ,NT posx,NT posy)
  {
    NT s1=s,s2=NT(2.0/3.0)*s;

    Digits::hor_particle_d<NT>()(s,posx,posy+s/NT(1.2));
    Digits::vert_particle_l<NT>()(s/NT(2.0),posx,posy+s/NT(1.2));
    Digits::hor_particle_d<NT>()(s,posx+1.4*s,posy+s/NT(1.2));
    Digits::vert_particle_r<NT>()(s/NT(2.0),posx+2.9*s,posy+NT(1.0/12.0)+s/NT(1.2));
  }

};

} // L_PAR

#endif //L_PAR_H
