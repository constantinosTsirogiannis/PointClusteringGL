#ifndef HOR_PARTICLE_D_H
#define HOR_PARTICLE_D_H

#include<GL/gl.h>
#include<math.h>
#include<GL/glut.h>
#include <cstdlib>
#include <iostream>

  
  namespace Digits {

template < class _NT >
class hor_particle_d {

  typedef _NT NT;

  public:
 
  hor_particle_d(){}

  void  
  operator()( NT s , NT posx, NT posy)
  {
    NT s1=s,s2=NT(2.0/3.0)*s;

    glBegin(GL_POLYGON);
    glVertex2f(posx+s1*0.0,posy+s2*0.0);
    glVertex2f(posx+s1*0.2,posy+s2*0.2);
    glVertex2f(posx+s1*1.2,posy+s2*0.2);
    glVertex2f(posx+s1*1.4,posy+s2*0.0);
    glEnd();
  }

};

} // hor_particle_d

#endif //HOR_PARTICLE_D_H
