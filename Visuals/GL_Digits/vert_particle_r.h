#ifndef VERT_PARTICLE_R_H
#define VERT_PARTICLE_R_H

#include<GL/gl.h>
#include<math.h>
#include<GL/glut.h>
#include <cstdlib>
#include <iostream>

  
  namespace Digits {

template < class _NT >
class vert_particle_r {

  typedef _NT NT;

  public:
 
  vert_particle_r(){}

  void  
  operator()( NT s , NT posx, NT posy)
  {
    NT s1=s,s2=NT(2.0/3.0)*s;

    glBegin(GL_POLYGON);
    glVertex2f(posx+s1*0.0,posy-s2*0.7);
    glVertex2f(posx+s1*0.0,posy+s2*0.7);
    glVertex2f(posx-s1*0.2,posy+s2*0.5);
    glVertex2f(posx-s1*0.2,posy-s2*0.5);
    glEnd();
  }

};

} // Digits

#endif //VERT_PARTICLE_R_H
